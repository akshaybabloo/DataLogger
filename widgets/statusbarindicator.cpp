#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include <QIcon>
#include <QDebug>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::StatusBarIndicator) {
    ui->setupUi(this);

    // load default icons
    checkIcon.load(":/images/check-icon.svg");
    xIcon.load(":/images/x-icon.svg");
    errorIcon.load(":/images/error-icon.svg");

    ui->bluetoothStatus->setPixmap(checkIcon.scaledToHeight(10, Qt::SmoothTransformation));
    ui->bluetoothStatus->setToolTip(tr("test"));

    ui->serverStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
    ui->serverStatus->setToolTip(tr("server not running"));

    ui->serverStatusLabel->clear();
}

StatusBarIndicator::~StatusBarIndicator() {
    delete ui;
}

void StatusBarIndicator::receiveServerStatusLabel(const QString &text) {
    if (text == ""){
        ui->serverStatusLabel->clear();
        ui->serverStatus->setPixmap(xIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->serverStatus->setToolTip(tr("server not running"));
    } else {
        ui->serverStatusLabel->setText(text);
        ui->serverStatus->setPixmap(checkIcon.scaledToHeight(10, Qt::SmoothTransformation));
        ui->serverStatus->setToolTip(tr("server running"));
    }
}
