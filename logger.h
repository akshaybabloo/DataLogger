#ifndef LOGGER_H
#define LOGGER_H

#include <QMainWindow>
#include <QtCharts>

using namespace QtCharts;

namespace Ui {
class Logger;
}

class Logger : public QMainWindow
{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger();

private:
    Ui::Logger *ui;
};

#endif // LOGGER_H
