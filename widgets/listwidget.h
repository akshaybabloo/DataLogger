#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>

namespace Ui {
    class ListWidget;
}

class ListWidget : public QWidget {
Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);

    ~ListWidget();

    void setText(const QString &text);

    QString getText();

private:
    Ui::ListWidget *ui;
};

#endif // LISTWIDGET_H
