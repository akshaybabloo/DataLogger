//
// Created by Akshay Raj Gollahalli on 31/05/21.
//

#include "chart.h"
#include <QDebug>
#include <QRandomGenerator>

Chart::Chart(QObject *parent) : QObject(parent) {
    dataUpdater.setInterval(0);
    dataUpdater.setSingleShot(true);
    QObject::connect(&dataUpdater, &QTimer::timeout, this, &Chart::updateAllSeries);
}

void Chart::startUpdating(const QList<QLineSeries *> &seriesList, const QVector<qreal> &values, qreal windowWidth,
                          qreal frequency) {

    // this will wait to get the frequency first
    if (frequency == 0) {
        return;
    }

    c_seriesList = seriesList;

//    qreal xAdjustment = 20.0 / (10 * values.length());
//    qreal yMultiplier = 3.0 / qreal(values.length());
    valueCounter += 1;
//    qInfo() << windowWidth;

//    for (int i = 0; i < values.length(); i++) {
//        QVector<QPointF> data;
//        for (int j = 0; j < seriesList.length(); ++j) {
//            data.append(QPointF(valueCounter, values[i]));
//        }
//        m_data.append(data);
//    }
//    qInfo() << m_data;

    tempData.append(values);

//    for (int i = 0; i < values.length(); ++i) {
////        seriesList[i]->clear();
////    seriesList[i].remo
////
////        seriesList[i]->append(QPointF(2.5, QRandomGenerator::global()->bounded(5) - 2.5));
////        qInfo() << seriesList[i]->points().length();
//
////        qInfo() << valueCounter;
//        m_data.append(QPointF(valueCounter, values[i]));
//    }
//    qInfo() << m_data.length() << valueCounter;
//    if (valueCounter > windowWidth) {
//        m_data.removeFirst();
//        valueCounter = 0;
//    }

    if (valueCounter == frequency) {
        dataUpdater.start();
//        qInfo() << tempData;
        valueCounter = 0;
    }
}

void Chart::updateAllSeries() {
    pointCounter += 1;

    qInfo() << "updates called";
    qInfo() << tempData.length();

    tempData.clear();

//    for (int i = 0; i < c_seriesList.length(); ++i) {
//        c_seriesList[i]->replace(m_data);
//    }
}
