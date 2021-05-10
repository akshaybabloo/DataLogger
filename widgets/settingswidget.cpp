#include "settingswidget.h"
#include "ui_settingswidget.h"

#include <QDebug>

SettingsWidget::SettingsWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SettingsWidget) {
    ui->setupUi(this);

    ui->tabWidget->setTabText(ui->tabWidget->indexOf(ui->uiTab), tr("UI"));

    auto theme = settings.value("theme", "light").toString();

    if (theme == "dark") {
        ui->darkButton->setChecked(true);
    } else if (theme == "light") {
        ui->lightButton->setChecked(true);
    } else if (theme == "native") {
        ui->nativeButton->setChecked(true);
    } else {
        ui->lightButton->setChecked(true);
    }
}

SettingsWidget::~SettingsWidget() {
    delete ui;
}

void SettingsWidget::on_buttonBox_rejected() {
    this->close();
}


void SettingsWidget::on_buttonBox_accepted() {

    if (ui->darkButton->isChecked()) {
        settings.setValue("theme", "dark");
    } else if (ui->lightButton->isChecked()) {
        settings.setValue("theme", "light");
    } else if (ui->nativeButton->isChecked()) {
        settings.setValue("theme", "native");
    } else {
        settings.setValue("theme", "light");
    }

    this->close();
}

