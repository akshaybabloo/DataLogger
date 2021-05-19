#include "listwidget.h"
#include "ui_listwidget.h"
#include "deviceinfo.h"
#include <QDebug>

ListWidget::ListWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ListWidget) {
    ui->setupUi(this);

#ifdef Q_OS_WINDOWS
    ui->verticalLayout->addSpacing(6);
#endif
}

ListWidget::~ListWidget() {
    delete ui;
}

void ListWidget::setText(const QString &text) {
    ui->deviceLabel->setText(text);
}

QString ListWidget::getText() {
    return ui->deviceLabel->text();
}

void ListWidget::setStatusText(const QString &text, bool isPaired) {
    ui->statusLabel->setText(text);
    if (isPaired) {
        ui->statusLabel->setStyleSheet("QLabel { color : green; }");
    } else {
        ui->statusLabel->setStyleSheet("QLabel { color : #F44336; }");
    }
}

void ListWidget::setDevice(const QBluetoothDeviceInfo &info) {
    bluetoothInfo = info;
}

QBluetoothDeviceInfo ListWidget::getDevice() {
    return bluetoothInfo;
}
