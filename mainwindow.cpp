#include <QListWidgetItem>
#include <widgets/listwidget.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "devices.h"
#include "logger.h"
#include "settings.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Creates a list of custom widgets
    for (int i = 0; i < 5; ++i) {
        auto item = new QListWidgetItem();

        auto widget = new ListWidget(this);
        widget->setText(QString("text %1").arg(i));

        item->setSizeHint(widget->sizeHint());

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto macId = dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(item))->getText();

    auto datalogger = new Logger();
    datalogger->setWindowTitle(macId);
    datalogger->show();

    this->close();
}
void MainWindow::on_actionSettings_triggered()
{
    auto settingsWindow = new Settings(this);
    settingsWindow->show();
}

