#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
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

    QString getText();

private:
    Ui::ListWidget *ui;
};

#endif // LISTWIDGET_H
