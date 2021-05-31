#include "channelwidget.h"
#include "ui_channelwidget.h"

ChannelWidget::ChannelWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ChannelWidget) {
    ui->setupUi(this);
}

ChannelWidget::~ChannelWidget() {
    delete ui;
}

void ChannelWidget::setText(const QString &text) {
    ui->checkBox->setText(text);
}

void ChannelWidget::setChecked(bool checked) {
    ui->checkBox->setChecked(checked);
}


