//
// Created by Akshay Raj Gollahalli on 31/05/21.
//

#include "chart.h"

using namespace std;

Chart::Chart(QObject *parent) : QObject(parent) {
    dataUpdater.setInterval(0);
    dataUpdater.setSingleShot(true);
    QObject::connect(&dataUpdater, &QTimer::timeout, this, &Chart::updateAllSeries);

    matrix = new MatrixXd(250, 24);
}

void Chart::startUpdating(const QList<QLineSeries *> &seriesList, const QVector<qreal> &values, qreal windowWidth,
                          qreal frequency) {

    c_seriesList = seriesList;
    c_windowWidth = windowWidth;

    // this will wait to get the frequency first
    if (frequency == 0) {
        return;
    }

    auto valuesVector = std::vector<qreal>(values.begin(), values.end());
    RowVectorXd vector = Map<RowVectorXd, Unaligned>(valuesVector.data(), (long) valuesVector.size());

    matrix->row(valueCounter) = vector;

    xAdjustment = 20.0 / (10 * values.length());
    yMultiplier = 3.0 / qreal(values.length());

    valueCounter += 1;

    if (valueCounter == frequency) {
        dataUpdater.start();
        valueCounter = 0;
    }
}

void Chart::updateAllSeries() {

    removeZeroRows(*matrix);
    RowVectorXd meanData = matrix->colwise().mean().normalized();

    for (int i = 0; i < meanData.size(); i++) {
        QVector<QPointF> data;
        for (int j = 0; j < c_seriesList.length(); ++j) {
            data.append(QPointF(pointCounter / 2, meanData[i]));
        }
        m_data.append(data);
    }

    for (auto &i : c_seriesList) {
        i->replace(m_data);
    }

    pointCounter += 1;

    if (pointCounter/2 > 10) {
        m_data.clear();
        pointCounter = 0;
    }

    delete matrix;
    matrix = nullptr;
    matrix = new MatrixXd(250, 24);
}

void Chart::removeZeroRows(Eigen::MatrixXd &mat) {
    Matrix<bool, Dynamic, 1> empty = (mat.array() == 0).rowwise().all();

    auto last = mat.rows() - 1;
    for (long i = 0; i < last + 1;) {
        if (empty(i)) {
            mat.row(i).swap(mat.row(last));
            empty.segment<1>(i).swap(empty.segment<1>(last));
            --last;
        } else {
            ++i;
        }
    }
    mat.conservativeResize(last + 1, mat.cols());
}
