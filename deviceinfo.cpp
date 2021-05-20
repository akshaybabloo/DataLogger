//
// Created by em8273 on 19/05/2021.
//

#include "deviceinfo.h"
#include <qbluetoothaddress.h>
#include <qbluetoothuuid.h>

DeviceInfo::DeviceInfo(const QBluetoothDeviceInfo &deviceInfo) {
    device = deviceInfo;
}

QString DeviceInfo::getAddress() const {
#ifdef Q_OS_MAC
    return device.deviceUuid().toString();
#else
    return device.address().toString();
#endif
}

QString DeviceInfo::getName() const {
    return device.name();
}

QBluetoothDeviceInfo DeviceInfo::getDevice() {
    return device;
}


