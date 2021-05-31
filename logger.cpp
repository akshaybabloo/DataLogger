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
#include <widgets/channelwidget.h>

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

    qChart = new QChart();
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    qChart->addAxis(axisX, Qt::AlignBottom);
    qChart->addAxis(axisY, Qt::AlignLeft);

    axisX->setRange(0,10.0);
    axisX->setTickCount(10);
    axisY->setRange(-2.5,2.5);

    chart = new Chart(qChart);

    auto *chartView = new QChartView(qChart);
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
    delete qChart;
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
        hexValue = hexValue + QString("%1").arg(QString::number(abs(value[i]), 16)) +
                   QString("%1").arg(abs(value[i + 1]), 2, 16, QLatin1Char('0')) + " ";
    }
    hexValue = hexValue.trimmed();

    QList<qreal> values;
    auto hexValueArray = hexValue.split(" ");
    for (int i = 0; i < hexValueArray.length(); ++i) {
        auto nValue = -hexValueArray[i].toInt(nullptr,
                                              16); // negating value because this gives me positive value (according to c# code)
        auto vValue = static_cast<qreal>(((nValue * 5) / 65536.0));
        values.append(vValue);
    }

    qInfo() << values;
    if (lineSeries.length() == 0) {
        qChart->legend()->hide();
        QPen red(Qt::yellow);
        red.setWidth(3);
        for (int i = 0; i < values.length(); ++i) {
            auto *series = new QLineSeries;
            series->setUseOpenGL(true);
            series->setPen(red);
            lineSeries.append(series);

            qChart->addSeries(series);

            series->attachAxis(axisX);
            series->attachAxis(axisY);

            // set channel widgets to channel list
            auto item = new QListWidgetItem();
            auto widget = new ChannelWidget(this);
            widget->setText(QString("Channel %1").arg(i+1));
            widget->setChecked(true);
            item->setSizeHint(widget->sizeHint());
            ui->channelListWidget->addItem(item);
            ui->channelListWidget->setItemWidget(item, widget);
        }
    }

    qreal x = qChart->plotArea().width() / axisX->tickCount();
    chart->startUpdating(lineSeries, values, x);
//    qInfo() << qChart->plotArea().width();
//    qChart->scroll(x, 0);
}

void Logger::confirmedDescriptorWrite(const QLowEnergyDescriptor &info, const QByteArray &value) {
    if (info.isValid() && info == channelSubscribeDesc && value == QByteArray::fromHex("0000")) {
        //disabled notifications -> assume disconnect intent
        controller->disconnectFromDevice();
        delete channelSubscribeService;
        channelSubscribeService = nullptr;
    }
}

void Logger::on_saveToFileButton_clicked() {

}

