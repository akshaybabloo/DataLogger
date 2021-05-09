#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QSettings>

namespace Ui {
    class SettingsWidget;
}

class SettingsWidget : public QWidget {
Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);

    ~SettingsWidget();

private slots:

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    QSettings settings;
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
