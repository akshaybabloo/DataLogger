#include <QListWidgetItem>
#include <widgets/listwidget.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "devices.h"
#include "logger.h"
#include "widgets/settingswidget.h"
#include<QDebug>
#include <widgets/statusbarindicator.h>
#include "widgets/aboutwidget.h"

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

    auto *indicator = new StatusBarIndicator(this);
    ui->statusbar->addPermanentWidget(indicator, 1);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    auto macId = dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(item))->getText();

    auto datalogger = new Logger();
    datalogger->setWindowTitle(macId);
    datalogger->show();

    this->close();
}

void MainWindow::on_actionSettings_triggered() {
    auto settingsWindow = new SettingsWidget(this);
    settingsWindow->setWindowFlag(Qt::Tool);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}


void MainWindow::on_actionAbout_DataLogger_triggered()
{
    auto about = new AboutWidget(this);
    about->setWindowFlag(Qt::Tool);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}

