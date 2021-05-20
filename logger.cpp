#include "logger.h"
#include "ui_logger.h"
#include <QLineSeries>
#include "widgets/settingswidget.h"
#include "widgets/statusbarindicator.h"
#include "mainwindow.h"
#include <QDebug>
#include "deviceinfo.h"
#include <qlowenergycontroller.h>
#include <widgets/aboutwidget.h>

Logger::Logger(QWidget *parent, QBluetoothDeviceInfo *deviceInfo) :
        QMainWindow(parent),
        ui(new Ui::Logger) {
    ui->setupUi(this);

    setWindowTitle("Logger");

    qInfo() << deviceInfo->address();
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

    auto *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

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

