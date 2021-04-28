#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "devices.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto d = Devices(parent).isBluetoothEnabled();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Hello() {
}

