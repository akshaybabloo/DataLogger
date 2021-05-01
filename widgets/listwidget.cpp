#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ListWidget) {
    ui->setupUi(this);
    connect(this, SIGNAL(sendRemoveItem(const QString &)), parent, SLOT(removeItem(const QString &)));
}

ListWidget::~ListWidget() {
    delete ui;
}

void ListWidget::setText(const QString &text) {
    ui->label->setText(text);
}

void ListWidget::on_toolButton_clicked() {
    emit sendRemoveItem(ui->label->text());
}

QString ListWidget::getText() {
    return ui->label->text();
}