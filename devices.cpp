//
// Created by Akshay Raj Gollahalli on 28/04/21.
//

#include <QBluetoothLocalDevice>
#include <QDebug>
#include "devices.h"

Devices::Devices(QObject *parent) {
};

bool Devices::isBluetoothEnabled() {
    auto hostMode = QBluetoothLocalDevice().hostMode();
    if (hostMode == QBluetoothLocalDevice::HostPoweredOff) {
        return false;
    } else {
        return true;
    }
}
