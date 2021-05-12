#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include <QIcon>
#include <QDebug>
#include <QMovie>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::StatusBarIndicator) {
    ui->setupUi(this);

    // load default icons
    checkIcon.load(":/images/check-icon.svg");
    xIcon.load(":/images/x-icon.svg");
    errorIcon.load(":/images/error-icon.svg");

    if (parent->objectName() != "MainWindow") {
        ui->serverStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->serverStatus->setToolTip(tr("server not running"));
    } else {
        // TODO: connect this to bluetooth search
        auto *loadingIcon = new QMovie(":/images/loading-icon.gif");
        loadingIcon->setScaledSize(QSize(15, 15));
        ui->loadingIconLabel->setMovie(loadingIcon);
        loadingIcon->start();
        ui->loadingTextLabel->setText("searching...");
    }

    auto *device = new QBluetoothLocalDevice(this);
    connect(device, &QBluetoothLocalDevice::hostModeStateChanged, this, &StatusBarIndicator::bluetoothStatus);
    bluetoothStatus(device->hostMode());
}

StatusBarIndicator::~StatusBarIndicator() {
    delete ui;
}

void StatusBarIndicator::receiveServerStatusLabel(const QString &text) {
    if (text == "") {
        ui->serverStatusLabel->setText("");
        ui->serverStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->serverStatus->setToolTip(tr("server not running"));
    } else {
        ui->serverStatusLabel->setText(text);
        ui->serverStatus->setPixmap(checkIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->serverStatus->setToolTip(tr("server running"));
    }
}

void StatusBarIndicator::bluetoothStatus(QBluetoothLocalDevice::HostMode state) {
    qInfo() << state;
    if (state == QBluetoothLocalDevice::HostPoweredOff) {
        ui->bluetoothStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->bluetoothStatus->setToolTip(tr("Bluetooth device not found"));
    } else {
        ui->bluetoothStatus->setPixmap(checkIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->bluetoothStatus->setToolTip(tr("Bluetooth enabled"));
    }
}
