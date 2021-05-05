#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ListWidget) {
    ui->setupUi(this);
}

ListWidget::~ListWidget() {
    delete ui;
}

void ListWidget::setText(const QString &text) {
    ui->label->setText(text);
}

QString ListWidget::getText() {
    return ui->label->text();
}
