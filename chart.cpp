//
// Created by Akshay Raj Gollahalli on 31/05/21.
//

#include "chart.h"
#include <QDebug>

Chart::Chart(QObject *parent) : QObject(parent) {

}

void Chart::startUpdating(const QList<QLineSeries *> &seriesList, const QList<qreal> &values) {


    for (int i = 0; i < values.length(); ++i) {

    }
}