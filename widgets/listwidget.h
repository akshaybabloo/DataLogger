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

signals:

    void sendRemoveItem(const QString &text);

private slots:

    void on_toolButton_clicked();

private:
    Ui::ListWidget *ui;
};

#endif // LISTWIDGET_H
