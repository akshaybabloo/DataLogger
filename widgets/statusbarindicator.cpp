#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include <QIcon>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarIndicator)
{
    ui->setupUi(this);
    QPixmap bleCheck(":/images/check-icon.svg");

    ui->bluetoothStatus->setPixmap(bleCheck.scaled(10,10, Qt::KeepAspectRatio));
    ui->bluetoothStatus->setToolTip(tr("test"));

    ui->serverStatus->setPixmap(bleCheck.scaled(10,10, Qt::KeepAspectRatio));

}

StatusBarIndicator::~StatusBarIndicator()
{
    delete ui;
}
