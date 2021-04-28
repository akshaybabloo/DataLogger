/**
 * Devices helps in searching and initialising them to stream data
 */

#ifndef DATALOGGER_DEVICES_H
#define DATALOGGER_DEVICES_H

class Devices {

public:

    explicit Devices(QObject *parent);

    /**
     * isBluetoothEnabled checks if a bluetooth device exists and enabled.
     *
     * @return `true` if enabled else `false`
     */
    static bool isBluetoothEnabled();

};


#endif //DATALOGGER_DEVICES_H
