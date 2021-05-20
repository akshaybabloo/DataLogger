#ifndef LOGGER_H
#define LOGGER_H

#include <QMainWindow>
#include <QtCharts>
#include <qlowenergycontroller.h>
#include <widgets/statusbarindicator.h>
#include "deviceinfo.h"

using namespace QtCharts;

namespace Ui {
    class Logger;
}

class Logger : public QMainWindow {
Q_OBJECT
    Q_PROPERTY(bool useRandomAddress READ isRandomAddress WRITE setRandomAddress NOTIFY randomAddressChanged)
public:
    explicit Logger(QWidget *parent = nullptr, QBluetoothDeviceInfo *deviceInfo = nullptr);

    ~Logger();

public slots:
    void slotReboot();

signals:
    void emitServerStatusLabel(const QString &text);
    void randomAddressChanged();

private slots:
    void deviceConnected();
    void deviceDisconnected();
    void addLowEnergyService(const QBluetoothUuid &serviceUUID);
    void serviceScanDone();
    void error(QLowEnergyController::Error newError);

    // TODO: change these to connect()
    void on_actionExit_triggered();
    void on_actionAbout_DataBlogger_triggered();
    void on_actionSettings_triggered();
    void on_serverButton_toggled(bool checked);

private:
    bool isRandomAddress();
    void setRandomAddress(bool newValue);

private:
    QSettings settings;
    QLowEnergyController *controller = nullptr;
    StatusBarIndicator *indicator;
    QList<QObject *> services;

    Ui::Logger *ui;

    bool randomAddress = false;
};

#endif // LOGGER_H
