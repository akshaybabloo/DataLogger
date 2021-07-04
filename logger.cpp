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
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &Logger::countFrequency);

    controller = QLowEnergyController::createCentral(*deviceInfo);
    connect(controller, &QLowEnergyController::connected, this, &Logger::deviceConnected);
    connect(controller, &QLowEnergyController::disconnected, this, &Logger::deviceDisconnected);
    connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error), this, &Logger::error);
#ifdef Q_OS_WINDOWS
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &Logger::addLowEnergyService,
            Qt::QueuedConnection);
#else
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &Logger::addLowEnergyService);
#endif
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

    axisX->setRange(0, 10.0);
    axisX->setTickCount(30);
    axisY->setRange(-30.0, 30.0);

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
    delete channelSubscribeService;
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
        udpSocket = new QUdpSocket(this);
        ui->serverButton->setText("Stop Server");
        emit emitServerStatusLabel("server running on 127.0.0.1:1000");
    } else {
        udpSocket->close();
        ui->serverButton->setText("Start Server");
        emit emitServerStatusLabel("");
    }

    doStream = checked;
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
    qInfo() << "New service discovered";
    QLowEnergyService *service = controller->createServiceObject(serviceUUID);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    qInfo() << "Service UUID:" << serviceUUID;
    services.append(new ServiceInfo(service));
    // TODO: get the service information

    if (serviceUUID == QBluetoothUuid(ChannelDataServiceUUID)) {
        qInfo() << "Channel data service found";
        foundDataChannelService = true;
    }
}

void Logger::serviceScanDone() {
    qInfo() << "service scan done";

    if (foundDataChannelService) {
        connectToService(ChannelDataServiceUUID);
    }
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

    qInfo() << "Data service found" << serviceUUID;
    if (foundDataChannelService) {
        channelSubscribeService = service;
    }

    if (channelSubscribeService) {
        qInfo() << "service object created";
        connect(channelSubscribeService, &QLowEnergyService::stateChanged, this, &Logger::serviceStateChanged);
        connect(channelSubscribeService, &QLowEnergyService::characteristicChanged, this, &Logger::updateWaveValue);
        connect(channelSubscribeService, &QLowEnergyService::descriptorWritten, this,
                &Logger::confirmedDescriptorWrite);
        connect(channelSubscribeService, QOverload<QLowEnergyService::ServiceError>::of(&QLowEnergyService::error),
                this, &Logger::errorService);

#ifdef Q_OS_WINDOWS
        // See https://forum.qt.io/topic/127222/issue-with-bluetooth-le-service-discovery-qlowenergyservice-unknownerror
        QTimer::singleShot(0, [=]() {
            channelSubscribeService->discoverDetails();
        });
#else
        channelSubscribeService->discoverDetails();
#endif
    }
}

void Logger::serviceStateChanged(QLowEnergyService::ServiceState newState) {
    qInfo() << "New state:" << newState;
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
            if (channelSubscribeDesc.isValid()) {
                //qInfo() << tr("Service discovered.");
                channelSubscribeService->writeDescriptor(channelSubscribeDesc, QByteArray::fromHex("0100"));
                timer->start(1000);
            }

            break;
        }
//        case QLowEnergyService::DiscoveryRequired: {
//            channelSubscribeService->discoverDetails();
//        }
        default:
            //nothing for now
            break;
    }
}

void Logger::updateWaveValue(const QLowEnergyCharacteristic &info, const QByteArray &value) {

    if (!frequencyCheckDone) {
        frequencyCounter++;
    }

    QString hexValue = "";
    for (int i = 4; i < value.length(); i += 2) {
        // using abs because value[i] produces negative value (according to c# code)
        hexValue = hexValue + QString("%1").arg(QString::number(abs(value[i]), 16)) +
                   QString("%1").arg(abs(value[i + 1]), 2, 16, QLatin1Char('0')) + " ";
    }
    hexValue = hexValue.trimmed();

    QVector<qreal> realValues;
    auto hexValueArray = hexValue.split(" ");
    for (int i = 0; i < hexValueArray.length(); ++i) {
        auto nValue = -hexValueArray[i].toInt(nullptr,
                                              16); // negating value because this gives me positive value (according to c# code)
        auto vValue = static_cast<qreal>(((nValue * 5) / 65536.0));
        realValues.append(vValue);
    }

//    qInfo() << values;
    if (lineSeries.length() == 0) {
        qChart->legend()->hide();
        QPen red(Qt::yellow);
        red.setWidth(3);
        for (int i = 0; i < realValues.length(); ++i) {
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
            widget->setText(QString("Channel %1").arg(i + 1));
            widget->setChecked(true);
            item->setSizeHint(widget->sizeHint());
            ui->channelListWidget->addItem(item);
            ui->channelListWidget->setItemWidget(item, widget);
        }
    }

//    qreal x = qChart->plotArea().width() / axisX->tickCount();
    chart->startUpdating(lineSeries, realValues, qChart->plotArea().width(), frequencyCounter);
//    qInfo() << qChart->plotArea().width();
//    qChart->scroll(x, 0);

    if (doStream) {
        udpSocket->writeDatagram(reinterpret_cast<const char *>(realValues.constData()), realValues.size(), QHostAddress::Broadcast, 4545);
    }
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

void Logger::countFrequency() {
    frequencyCheckDone = true;
    qInfo() << "frequency is " << frequencyCounter;
}

void Logger::closeEvent(QCloseEvent *event) {
    if (channelSubscribeDesc.isValid() && channelSubscribeService &&
        channelSubscribeDesc.value() == QByteArray::fromHex("0100")) {
        qInfo() << "Unsubscribing channel services";
        channelSubscribeService->writeDescriptor(channelSubscribeDesc, QByteArray::fromHex("0000"));
    } else {
        if (controller) {
            qInfo() << "Disconnecting from device";
            controller->disconnectFromDevice();
        }
        delete channelSubscribeService;
        channelSubscribeService = nullptr;
    }
}

void Logger::errorService(QLowEnergyService::ServiceError error) {
    qWarning() << "Service Error: " << error;
}

