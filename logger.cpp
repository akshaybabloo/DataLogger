#include "logger.h"
#include "ui_logger.h"

Logger::Logger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Logger)
{
    ui->setupUi(this);
}

Logger::~Logger()
{
    delete ui;
}
