//
// Created by Akshay Raj Gollahalli on 31/05/21.
//

#ifndef DATALOGGER_CHART_H
#define DATALOGGER_CHART_H

#include <QList>
#include <QObject>
#include <QtCharts/QLineSeries>
#include <qelapsedtimer.h>
#include <qtimer.h>

QT_CHARTS_USE_NAMESPACE

class Chart : public QObject {

Q_OBJECT
public:
    explicit Chart(QObject *parent = 0);
    void startUpdating(const QList<QLineSeries *> &seriesList, const QVector<qreal> &values, qreal windowWidth, qreal frequency);

private slots:
    void updateAllSeries();

private:
    QList<QLineSeries *> c_seriesList;
    QVector<QPointF> m_data;
    qreal valueCounter = 0;
    QElapsedTimer timer;
    QTimer dataUpdater;
    QVector<QVector<qreal>> tempData;
    qreal pointCounter = 0;


};


#endif //DATALOGGER_CHART_H
