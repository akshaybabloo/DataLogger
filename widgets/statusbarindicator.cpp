#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include "mainwindow.h"
#include <QIcon>
#include <QDebug>
#include <QMovie>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::StatusBarIndicator), localDevice(new QBluetoothLocalDevice) {
    ui->setupUi(this);

    // load default icons
    checkIcon.load(":/images/check-icon.svg");
    xIcon.load(":/images/x-icon.svg");
    errorIcon.load(":/images/error-icon.svg");

    if (parent->objectName() != "MainWindow") {
        ui->serverStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->serverStatus->setToolTip(tr("server not running"));
        ui->refreshButton->hide();
    } else {
        auto *loadingIcon = new QMovie(":/images/loading-icon.gif");
        loadingIcon->setScaledSize(QSize(15, 15));
        ui->loadingIconLabel->setMovie(loadingIcon);
        loadingIcon->start();
        ui->loadingTextLabel->setText("searching...");
        ui->batteryBar->hide();

        // clicking on the refresh button triggers bluetooth scanning
        connect(ui->refreshButton, SIGNAL(released()), parent, SLOT(scan()));
    }

    connect(localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &StatusBarIndicator::bluetoothStatus);

    bluetoothStatus(localDevice->hostMode());
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
    if (state == QBluetoothLocalDevice::HostPoweredOff) {
        ui->bluetoothStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->bluetoothStatus->setToolTip(tr("Bluetooth device not found"));
    } else {
        ui->bluetoothStatus->setPixmap(checkIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->bluetoothStatus->setToolTip(tr("Bluetooth enabled"));
    }
}

void StatusBarIndicator::scanStatus(bool finished, const QString &statusText) {
    if (finished) {
        ui->refreshButton->show();
        ui->refreshButton->setEnabled(true);
        ui->loadingTextLabel->hide();
        ui->loadingIconLabel->hide();
        if (statusText != nullptr) {
            ui->loadingTextLabel->setText(statusText);
            ui->loadingTextLabel->show();
        }
    } else {
        ui->refreshButton->hide();
        ui->refreshButton->setDisabled(true);
        ui->loadingTextLabel->show();
        ui->loadingIconLabel->show();
        if (statusText != nullptr) {
            ui->loadingTextLabel->setText(statusText);
            ui->loadingTextLabel->show();
        }
    }
}