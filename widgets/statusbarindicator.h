#ifndef STATUSBARINDICATOR_H
#define STATUSBARINDICATOR_H

#include <QWidget>
#include <QPixmap>
#include <QBluetoothLocalDevice>
#include <QIcon>

namespace Ui {
    class StatusBarIndicator;
}

class StatusBarIndicator : public QWidget {
Q_OBJECT

public:
    explicit StatusBarIndicator(QWidget *parent = nullptr);

    ~StatusBarIndicator();

public slots:

    void receiveServerStatusLabel(const QString &text);

    void bluetoothStatus(QBluetoothLocalDevice::HostMode state);

    void scanStatus(bool finished, const QString &statusText = nullptr);

private:
    QPixmap checkIcon;
    QPixmap xIcon;
    QPixmap errorIcon;
    QIcon refreshIcon;
    QBluetoothLocalDevice *localDevice;
    Ui::StatusBarIndicator *ui;
};

#endif // STATUSBARINDICATOR_H
