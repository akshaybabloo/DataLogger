/**
 * This is an entry window
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    static int const EXIT_CODE_REBOOT;

    ~MainWindow() override;

public slots:

    void slotReboot();
    void scan();

signals:
    void scanStatus(bool finished, const QString& statusText = nullptr);

private slots:

    /**
     * When an item list is double clicked, this method is invoked.
     *
     * @param item An widget item
     */
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    /**
     * Opens setting window
     */
    void on_actionSettings_triggered();

    void on_actionAbout_DataLogger_triggered();

    void on_actionExit_triggered();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    Ui::MainWindow *ui;
    QBluetoothLocalDevice *localDevice;
    QSettings settings;
    bool filterBle;

private:
    /**
     * This method is triggered when a new device is found, then the device will be added to the list.
     *
     * @param info device information
     */
    void addDevice(const QBluetoothDeviceInfo &info);

    /**
     * Checks if a device already exists in the QListWidget
     *
     * @param label name of the device to check
     * @return is present return `true` else `false`
     */
    bool isDeviceExists(const QString &label);

    /**
     * Triggers when the scanning for Bluetooth is finished
     */
    void scanFinished();

    /**
     * Opens a pairing menu with options for pairing and removing a bluetooth device.
     *
     * @param pos position of right click
     */
    void pairingMenu(const QPoint &pos);
};

#endif // MAINWINDOW_H
