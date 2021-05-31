#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>

namespace Ui {
    class ChannelWidget;
}

class ChannelWidget : public QWidget {
Q_OBJECT

public:
    explicit ChannelWidget(QWidget *parent = nullptr);
    ~ChannelWidget();

    void setText(const QString &text);
    void setChecked(bool checked = true);

private:
    Ui::ChannelWidget *ui;
};

#endif // CHANNELWIDGET_H
