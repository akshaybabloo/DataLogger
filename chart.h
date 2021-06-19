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
#include <Eigen/Dense>

QT_CHARTS_USE_NAMESPACE
using namespace Eigen;

class Chart : public QObject {

Q_OBJECT
public:
    explicit Chart(QObject *parent = 0);

    /**
     * This method is called by the Bluetooth stream API
     *
     * @param seriesList List of the \see Qt::QtCharts::QLineSeries
     * @param values A \see QVector of the streamed values
     * @param windowWidth Width of the UI window
     * @param frequency Frequency of the streaming. This will change depending on the OS and the hardware
     */
    void startUpdating(const QList<QLineSeries *> &seriesList, const QVector<qreal> &values, qreal windowWidth, qreal frequency);

private slots:
    /**
     * Updates the mean values to the chart
     */
    void updateAllSeries();

private:
    QList<QLineSeries *> c_seriesList;
    QVector<QPointF> m_data;
    qint32 valueCounter = 0;
    QElapsedTimer timer;
    QTimer dataUpdater;
    QVector<QVector<qreal>> tempData;
    qreal pointCounter = 0;
    MatrixXd *matrix = nullptr;

private:
    /**
     * Removes an row with 0s in it.
     * @param mat new MatrixXd
     */
    void removeZeroRows(Eigen::MatrixXd& mat);

};


#endif //DATALOGGER_CHART_H
