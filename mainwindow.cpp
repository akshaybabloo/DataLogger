#include <QListWidgetItem>
#include <widgets/listwidget.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "devices.h"
#include "logger.h"

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

void MainWindow::openLogger(const QString &text) {

    auto datalogger = new Logger();

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        auto item = ui->listWidget->item(i);
        auto itemWidget = dynamic_cast<ListWidget *>(ui->listWidget->itemWidget(item));
        if (itemWidget->getText() == text) {
            delete item;
            datalogger->show();
            this->close();
            break;
        }
    }
}
