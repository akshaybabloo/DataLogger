#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include <QIcon>
#include <QDebug>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarIndicator)
{
    ui->setupUi(this);
    QPixmap bleCheck(":/images/check-icon.svg");

    ui->bluetoothStatus->setPixmap(bleCheck.scaledToHeight(10, Qt::SmoothTransformation));
    ui->bluetoothStatus->setToolTip(tr("test"));

    ui->serverStatus->setPixmap(bleCheck.scaledToHeight(10, Qt::SmoothTransformation));

    ui->serverStatusLabel->setText("");
}

StatusBarIndicator::~StatusBarIndicator()
{
    delete ui;
}

void StatusBarIndicator::receiveServerStatusLabel(const QString &text) {
    qInfo() << text;
    ui->serverStatusLabel->setText(text);
}
