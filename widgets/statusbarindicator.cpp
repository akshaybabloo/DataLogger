#include "statusbarindicator.h"
#include "ui_statusbarindicator.h"
#include <QIcon>

StatusBarIndicator::StatusBarIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarIndicator)
{
    ui->setupUi(this);

    ui->bluetoothStatus->setPixmap(QPixmap(":theme/dark/primary/rightarrow.svg"));
    ui->serverStatus->setPixmap(QPixmap(":theme/dark/primary/rightarrow.svg"));

}

StatusBarIndicator::~StatusBarIndicator()
{
    delete ui;
}
