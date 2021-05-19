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

private slots:

    void on_actionSettings_triggered();

    void on_serverButton_toggled(bool checked);

signals:
    void emitServerStatusLabel(const QString &text);
    void randomAddressChanged();

private slots:
    void deviceConnected();
    void deviceDisconnected();
    void addLowEnergyService(const QBluetoothUuid &serviceUUID);
    void serviceScanDone();
    void error(QLowEnergyController::Error newError);

    void on_actionExit_triggered();

    void on_actionAbout_DataBlogger_triggered();

private:
    QSettings settings;
    QLowEnergyController *controller = nullptr;
    StatusBarIndicator *indicator;

    Ui::Logger *ui;

    bool isRandomAddress();
    void setRandomAddress(bool newValue);
    bool randomAddress = false;
};

#endif // LOGGER_H
