//
// Created by Akshay Raj Gollahalli on 31/05/21.
//

#ifndef DATALOGGER_CHART_H
#define DATALOGGER_CHART_H

#include <QList>
#include <QObject>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class Chart : public QObject {

    Q_OBJECT
public:
    explicit Chart(QObject *parent = 0);
    void startUpdating(const QList<QLineSeries *> &seriesList, const QList<qreal>& values);

//private slots:
//    void update(const QLineSeries *seriesList);

private:
    QList<QLineSeries *> c_seriesList;

};


#endif //DATALOGGER_CHART_H
