#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QBluetoothDeviceInfo>
#include "qglobal.h"


namespace Ui {
    class ListWidget;
}

class ListWidget : public QWidget {
Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);

    ~ListWidget();

    void setText(const QString &text);
    void setStatusText(const QString &text, bool isPaired=false);
    void setDevice(const QBluetoothDeviceInfo &info);
    QBluetoothDeviceInfo getDevice();
    QString getText();

private:
    Ui::ListWidget *ui;
    QBluetoothDeviceInfo bluetoothInfo;
};

#endif // LISTWIDGET_H
