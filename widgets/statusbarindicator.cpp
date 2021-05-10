#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include <QIcon>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarIndicator)
{
    ui->setupUi(this);
    QPixmap bleCheck(":/images/check-icon.svg");

    ui->bluetoothStatus->setPixmap(bleCheck.scaledToHeight(10, Qt::SmoothTransformation));
    ui->bluetoothStatus->setToolTip(tr("test"));

    ui->serverStatus->setPixmap(bleCheck.scaledToHeight(10, Qt::SmoothTransformation));

}

StatusBarIndicator::~StatusBarIndicator()
{
    delete ui;
}
