#include <QListWidgetItem>
#include <widgets/listwidget.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qglobal.h"
#include "logger.h"
#include "widgets/settingswidget.h"
#include <widgets/statusbarindicator.h>
#include "widgets/aboutwidget.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), localDevice(new QBluetoothLocalDevice) {
    ui->setupUi(this);

#ifdef Q_OS_WINDOWS
    ui->centralwidget->setContentsMargins(QMargins(0, 6, 0, 0));
#endif
    
    filterBle = settings.value("connectivity/enableBle", true).toBool();

    auto *indicator = new StatusBarIndicator(this);
    ui->statusbar->addPermanentWidget(indicator, 1);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    // whenever a device is discovered, this signal is triggered
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &MainWindow::addDevice);

    // activates when bluetooth scan is finished
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &MainWindow::scanFinished);

    // depending on the scan status the status indicators are hidden or shown
    connect(this, &MainWindow::scanStatus, indicator, &StatusBarIndicator::scanStatus);

    // adds custom context menu
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::pairingMenu);

    // start scanning
    scan();
}

MainWindow::~MainWindow() {
    delete discoveryAgent;
    delete localDevice;
    delete ui;
}

int const MainWindow::EXIT_CODE_REBOOT = -123456789;

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    auto device = dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(item))->getDevice();

    auto datalogger = new Logger(nullptr, &device);
    datalogger->show();

    discoveryAgent->stop();

    this->close();
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
    if (filterBle && !isDeviceExists(label) && info.coreConfigurations() && QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        auto item = new QListWidgetItem();

        auto widget = new ListWidget(this);
        widget->setText(label);
        widget->setDevice(info);

        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        if (pairingStatus == QBluetoothLocalDevice::Paired ||
            pairingStatus == QBluetoothLocalDevice::AuthorizedPaired) {
            widget->setStatusText("paired", true);
        } else {
            widget->setStatusText("not paired");
        }

#ifdef Q_OS_MAC
        item->setSizeHint(widget->sizeHint());
#endif

#ifdef Q_OS_WINDOWS
        item->setSizeHint(QSize(305, 65));
#endif

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

bool MainWindow::isDeviceExists(const QString &label) {

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        auto item = ui->listWidget->item(i);
        if (dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(item))->getText() == label) {
            return true;
        }
    }

    return false;
}

void MainWindow::scan() {
    emit scanStatus(false);
    ui->listWidget->clear();

    if (filterBle) {
        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    } else {
        discoveryAgent->start();
    }
}

void MainWindow::scanFinished() {
    emit scanStatus(true);
    qDebug() << "scan finished";
}

void MainWindow::pairingMenu(const QPoint &pos) {
    if (ui->listWidget->count() == 0) {
        return;
    }

    QMenu menu(this);
    QAction *pairAction = menu.addAction("Pair");
    QAction *removePairAction = menu.addAction("Remove pair");
    QAction *chosenAction = menu.exec(ui->listWidget->viewport()->mapToGlobal(pos));
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    auto widgetText = dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(currentItem))->getText();
    auto macId = widgetText.split('(')[0].trimmed();

    QBluetoothAddress address(macId);
    if (chosenAction == pairAction) {
        localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
    } else if (chosenAction == removePairAction) {
        localDevice->requestPairing(address, QBluetoothLocalDevice::Unpaired);
    }

}


void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

