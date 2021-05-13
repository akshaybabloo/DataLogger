/**
 * This is an entry window
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>

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

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

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

    Ui::MainWindow *ui;
    QBluetoothLocalDevice *localDevice;
};

#endif // MAINWINDOW_H
