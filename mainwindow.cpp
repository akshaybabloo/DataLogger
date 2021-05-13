#include <QListWidgetItem>
#include <widgets/listwidget.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "logger.h"
#include "widgets/settingswidget.h"
#include <widgets/statusbarindicator.h>
#include "widgets/aboutwidget.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto *indicator = new StatusBarIndicator(this);
    ui->statusbar->addPermanentWidget(indicator, 1);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->start();

    // whenever a device is discovered, this signal is triggered
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &MainWindow::addDevice);

}

MainWindow::~MainWindow() {
    delete discoveryAgent;
    delete ui;
}

int const MainWindow::EXIT_CODE_REBOOT = -123456789;

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    auto macId = dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(item))->getText();

    auto datalogger = new Logger(this);
    datalogger->setWindowTitle(macId);
    datalogger->show();

    this->hide();
}

void MainWindow::on_actionSettings_triggered() {
    auto settingsWindow = new SettingsWidget(this);
    settingsWindow->setWindowFlag(Qt::Tool);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}


void MainWindow::on_actionAbout_DataLogger_triggered() {
    auto about = new AboutWidget(this);
    about->setWindowFlag(Qt::Tool);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}

void MainWindow::slotReboot() {
    qDebug() << "Performing application reboot...";
    qApp->exit(MainWindow::EXIT_CODE_REBOOT);
}

void MainWindow::addDevice(const QBluetoothDeviceInfo &info) {
    QString label = QString("%1 (%2)").arg(info.address().toString(), info.name());
    QList<QListWidgetItem *> items = ui->listWidget->findItems(label, Qt::MatchExactly | Qt::MatchRecursive);
    // TODO: this will always return true because of the custom widget. Search by ListWidget and then filter it.
    if (items.empty()) {
        auto item = new QListWidgetItem();

        auto widget = new ListWidget(this);
        widget->setText(label);

        item->setSizeHint(widget->sizeHint());

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

