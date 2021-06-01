#ifndef LOGGER_H
#define LOGGER_H

#include <QMainWindow>
#include <QtCharts>
#include <qlowenergycontroller.h>
#include <widgets/statusbarindicator.h>
#include "deviceinfo.h"
#include "serviceinfo.h"
#include "chart.h"

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

    /**
     * Adds BLE services in this case its @see ChannelDataServiceUUID and @see DeviceControlServiceUUID
     * @param serviceUUID is the UUID of the service
     */
    void addLowEnergyService(const QBluetoothUuid &serviceUUID);
    void serviceScanDone();
    void error(QLowEnergyController::Error newError);

    // TODO: change these to connect()
    void on_actionExit_triggered();
    void on_actionAbout_DataBlogger_triggered();
    void on_actionSettings_triggered();
    void on_serverButton_toggled(bool checked);

    void serviceStateChanged(QLowEnergyService::ServiceState newState);
    void updateWaveValue(const QLowEnergyCharacteristic &info, const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &info, const QByteArray &value);

    void on_saveToFileButton_clicked();

    void countFrequency();

private:
    bool isRandomAddress();
    void setRandomAddress(bool newValue);
    void connectToService(const QString &serviceUUID);

private:
    QSettings settings;
    QLowEnergyController *controller = nullptr;
    StatusBarIndicator *indicator;
    QList<QObject *> services;
    QLowEnergyService *channelSubscribeService = nullptr;
    QLowEnergyDescriptor channelSubscribeDesc;
    QLowEnergyService *channelsService = nullptr;
    QChart *qChart = nullptr;
    Chart *chart = nullptr;
    QList<QLineSeries *> lineSeries;
    QValueAxis *axisX = nullptr;
    QValueAxis *axisY = nullptr;

    // get the stream frequency
    QTimer *timer = nullptr;
    quint32 frequencyCounter = 0;
    bool frequencyCheckDone = false;

    Ui::Logger *ui;

    bool randomAddress = false;

    // data services where data can be streamed from the device
    const QString ChannelDataServiceUUID = "5C872000-D20C-11EA-87D0-0242AC130003";
    const QString ChannelsUUID = "5C872001-D20C-11EA-87D0-0242AC130003"; // Read/write = 3 bytes, 1 bit for each channel
    const QString ChannelsSubscribeUUID = "5C872003-D20C-11EA-87D0-0242AC130003"; // Read/notify = 50 bytes. 2 bytes for msec counter. 48 bytes for data (2bytes/ch)

    // device information and controllers
    const QString DeviceControlServiceUUID = "5C873000-D20C-11EA-87D0-0242AC130003";
    const QString TimestampUUID = "5C873001-D20C-11EA-87D0-0242AC130003"; // Write = see BLE protocol doc (to sync time with system)
    const QString DeviceStatusUUID = "5C873002-D20C-11EA-87D0-0242AC130003"; // Read = see BLE protocol doc
    const QString FormatSdCardUUID = "5C873003-D20C-11EA-87D0-0242AC130003"; // Write = 0x01 (to format the card)
    const QString ADCLoggingUUID = "5C873004-D20C-11EA-87D0-0242AC130003"; // Read/Write = 0x01 (enable) or 0x00 (disable)
};

#endif // LOGGER_H
