//
// Created by Akshay Raj Gollahalli on 31/05/21.
//

#include "chart.h"
#include <QDebug>
#include <QRandomGenerator>

Chart::Chart(QObject *parent) : QObject(parent) {

}

void Chart::startUpdating(const QList<QLineSeries *> &seriesList, const QList<qreal> &values, qreal windowWidth) {

    qreal xAdjustment = 20.0 / (10 * values.length());
    qreal yMultiplier = 3.0 / qreal(values.length());


    for (int i = 0; i < values.length(); ++i) {
//        seriesList[i]->clear();
//    seriesList[i].remo
//
//        seriesList[i]->append(QPointF(2.5, QRandomGenerator::global()->bounded(5) - 2.5));
//        qInfo() << seriesList[i]->points().length();

        if (m_data.length() > windowWidth) {
            m_data.removeFirst();
        }

        m_data.append(QPointF(values[i]+i, QRandomGenerator::global()->bounded(5)));
    }

    for (int i = 0; i < values.length(); ++i) {
        seriesList[i]->replace(m_data);
    }
}