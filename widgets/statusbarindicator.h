#ifndef STATUSBARINDICATOR_H
#define STATUSBARINDICATOR_H

#include <QWidget>

namespace Ui {
class StatusBarIndicator;
}

class StatusBarIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBarIndicator(QWidget *parent = nullptr);
    ~StatusBarIndicator();

private:
    Ui::StatusBarIndicator *ui;
};

#endif // STATUSBARINDICATOR_H
