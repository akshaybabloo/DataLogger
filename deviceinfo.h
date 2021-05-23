/**
 * DeviceInfo class holds the information of a Bluetooth BLE device.
 */

#ifndef DATALOGGER_DEVICEINFO_H
#define DATALOGGER_DEVICEINFO_H


#include <QObject>
#include <qbluetoothdeviceinfo.h>

class DeviceInfo : public QObject {

public:
    DeviceInfo() = default;
    explicit DeviceInfo(const QBluetoothDeviceInfo &deviceInfo);

    /**
     * Gets the address of the device. On macOS this method returns UUID and on Windows it returns MAC ID.
     * @return QString of UUID or MAC ID
     */
    QString getAddress() const;

    /**
     * Returns name of the device
     * @return QString device name
     */
    QString getName() const;

    /**
     * Returns the Bluetooth device
     * @return QBluetoothDeviceInfo object
     */
    QBluetoothDeviceInfo getDevice();

private:
    QBluetoothDeviceInfo device;
};


#endif //DATALOGGER_DEVICEINFO_H
