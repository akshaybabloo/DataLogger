#ifndef LOGGER_H
#define LOGGER_H

#include <QMainWindow>
#include <QtCharts>
#include "deviceinfo.h"

using namespace QtCharts;

namespace Ui {
    class Logger;
}

class Logger : public QMainWindow {
Q_OBJECT

public:
    explicit Logger(QWidget *parent = nullptr, QBluetoothDeviceInfo *deviceInfo = nullptr);

    ~Logger();

public slots:

    void slotReboot();

private slots:

    void on_actionSettings_triggered();

    void on_serverButton_toggled(bool checked);

signals:

    void emitServerStatusLabel(const QString &text);

private:
    QSettings settings;

    Ui::Logger *ui;
};

#endif // LOGGER_H
