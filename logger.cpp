#include "logger.h"
#include "ui_logger.h"
#include <QLineSeries>
#include "widgets/settingswidget.h"
#include "widgets/statusbarindicator.h"
#include "mainwindow.h"
#include <QDebug>
#include "deviceinfo.h"
#include "serviceinfo.h"
#include <qlowenergycontroller.h>
#include <widgets/aboutwidget.h>

Logger::Logger(QWidget *parent, QBluetoothDeviceInfo *deviceInfo) :
        QMainWindow(parent),
        ui(new Ui::Logger) {
    ui->setupUi(this);

    setWindowTitle("Logger");
#ifdef Q_OS_MAC
    qInfo() << deviceInfo->deviceUuid();
#else
    qInfo() << deviceInfo->address();
#endif
    controller = QLowEnergyController::createCentral(*deviceInfo);
    connect(controller, &QLowEnergyController::connected, this, &Logger::deviceConnected);
    connect(controller, &QLowEnergyController::disconnected, this, &Logger::deviceDisconnected);
    connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error), this, &Logger::error);
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &Logger::addLowEnergyService);
    connect(controller, &QLowEnergyController::discoveryFinished, this, &Logger::serviceScanDone);

    if (isRandomAddress()) {
        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    } else {
        controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    }
    controller->connectToDevice();

    auto *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    series->setUseOpenGL(true);

    chart = new QChart();
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addSeries(series);
    chart->createDefaultAxes();

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto theme = settings.value("theme", "light").toString();
    if (theme == "dark") {
        chartView->chart()->setTheme(QtCharts::QChart::ChartThemeDark);
    }

    ui->rightLayout->addWidget(chartView);

    indicator = new StatusBarIndicator(this);
    ui->statusbar->addPermanentWidget(indicator, 1);
    connect(this, &Logger::emitServerStatusLabel, indicator, &StatusBarIndicator::receiveServerStatusLabel);
}

Logger::~Logger() {
    qDeleteAll(services);
    services.clear();
    controller->disconnectFromDevice();
    delete chart;
    delete controller;
    delete indicator;
    delete ui;
}

void Logger::on_actionSettings_triggered() {
    auto settingsWindow = new SettingsWidget(this);
    settingsWindow->setWindowFlag(Qt::Tool);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}

void Logger::on_serverButton_toggled(bool checked) {
    if (checked) {
        ui->serverButton->setText("Stop Server");
        emit emitServerStatusLabel("server running on 127.0.0.1:1000");
    } else {
        ui->serverButton->setText("Start Server");
        emit emitServerStatusLabel("");
    }
}

void Logger::slotReboot() {
    qDebug() << "Performing application reboot...";
    qApp->exit(MainWindow::EXIT_CODE_REBOOT);
}

void Logger::deviceConnected() {
    qInfo() << "device connected";
    controller->discoverServices();
}

void Logger::deviceDisconnected() {
    qInfo() << "device disconnected";
}

void Logger::addLowEnergyService(const QBluetoothUuid &serviceUUID) {
    qInfo() << "service discovered";
    QLowEnergyService *service = controller->createServiceObject(serviceUUID);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    qInfo() << serviceUUID;
    services.append(new ServiceInfo(service));
    // TODO: get the service information
}

void Logger::serviceScanDone() {
    qInfo() << "service scan done";

    connectToService(ChannelDataServiceUUID);
}

void Logger::error(QLowEnergyController::Error) {
    qWarning() << "Error: " << controller->errorString();
}

bool Logger::isRandomAddress() {
    return randomAddress;
}

void Logger::setRandomAddress(bool newValue) {
    randomAddress = newValue;
    emit randomAddressChanged();
}

void Logger::on_actionExit_triggered() {
    controller->disconnectFromDevice();
    QCoreApplication::quit();
}

void Logger::on_actionAbout_DataBlogger_triggered() {
    auto about = new AboutWidget(this);
    about->setWindowFlag(Qt::Tool);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}

void Logger::connectToService(const QString &serviceUUID) {
    qDebug() << "connect called";

    QLowEnergyService *service = nullptr;
    for (auto s : qAsConst(services)) {
        auto serviceInfo = qobject_cast<ServiceInfo *>(s);
        if (!serviceInfo) {
            continue;
        }
        if (serviceInfo->getUuid() == serviceUUID.toLower()) {
            service = serviceInfo->service();
            break;
        }
    }

    if (!service) {
        return;
    }

    qInfo() << serviceUUID;
    channelSubscribeService = controller->createServiceObject(QBluetoothUuid(serviceUUID), this);
    if (channelSubscribeService) {
        qInfo() << "service object created";
        connect(channelSubscribeService, &QLowEnergyService::stateChanged, this, &Logger::serviceStateChanged);
        connect(channelSubscribeService, &QLowEnergyService::characteristicChanged, this, &Logger::updateWaveValue);
//        connect(channelSubscribeService, &QLowEnergyService::characteristicRead, this, &Logger::updateWaveValue);
        connect(channelSubscribeService, &QLowEnergyService::descriptorWritten, this,
                &Logger::confirmedDescriptorWrite);
        channelSubscribeService->discoverDetails();
        return;
    }

    // this will list out services
    for (const auto &item : service->characteristics()) {
        qDebug() << item.uuid();
        qDebug() << item.value();
    }
}

void Logger::serviceStateChanged(QLowEnergyService::ServiceState newState) {
    qInfo() << newState;
    switch (newState) {
        case QLowEnergyService::DiscoveringServices:
            qInfo() << tr("Discovering services...");
            break;
        case QLowEnergyService::ServiceDiscovered: {
            qInfo() << tr("Service discovered.");

            const QLowEnergyCharacteristic energyCharacteristic = channelSubscribeService->characteristic(
                    QBluetoothUuid(ChannelsSubscribeUUID));
            if (!energyCharacteristic.isValid()) {
                qInfo() << tr("Data not found.");
                break;
            }

            channelSubscribeDesc = energyCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
            if (channelSubscribeDesc.isValid())
                channelSubscribeService->writeDescriptor(channelSubscribeDesc, QByteArray::fromHex("0100"));

            break;
        }
        default:
            //nothing for now
            break;
    }
}

void Logger::updateWaveValue(const QLowEnergyCharacteristic &info, const QByteArray &value) {
    QString hexValue = "";
    for (int i = 4; i < value.length(); i += 2) {
        // using abs because value[i] produces negative value (according to c# code)
        hexValue = hexValue + QString("%1").arg(QString::number(abs(value[i]), 16)) + QString("%1").arg(abs(value[i+1]), 2, 16, QLatin1Char('0')) + " ";
    }
    hexValue = hexValue.trimmed();

    QString intValue = "";
    auto hexValueArray = hexValue.split(" ");
    for (int i = 0; i < hexValueArray.length(); ++i) {
        auto nValue = -hexValueArray[i].toInt(nullptr, 16); // negating value because this gives me positive value (according to c# code)
        auto vValue = static_cast<double>(((nValue * 5) / 65536.0));
        intValue = intValue + QString::number(vValue) + " ";
    }
    intValue = intValue.trimmed();

    qInfo() << intValue;
}

void Logger::confirmedDescriptorWrite(const QLowEnergyDescriptor &info, const QByteArray &value) {
    if (info.isValid() && info == channelSubscribeDesc && value == QByteArray::fromHex("0000")) {
        //disabled notifications -> assume disconnect intent
        controller->disconnectFromDevice();
        delete channelSubscribeService;
        channelSubscribeService = nullptr;
    }
}

void Logger::on_saveToFileButton_clicked()
{
    auto *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    series->setUseOpenGL(true);
    chart->addSeries(series);
}

