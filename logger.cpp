#include "logger.h"
#include "ui_logger.h"
#include <QLineSeries>
#include "widgets/settingswidget.h"
#include "widgets/statusbarindicator.h"
#include "mainwindow.h"
#include <QDebug>

Logger::Logger(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Logger) {
    ui->setupUi(this);

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

    auto *indicator = new StatusBarIndicator(this);
    ui->statusbar->addPermanentWidget(indicator, 1);
    connect(this, &Logger::emitServerStatusLabel, indicator, &StatusBarIndicator::receiveServerStatusLabel);
}

Logger::~Logger() {
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
