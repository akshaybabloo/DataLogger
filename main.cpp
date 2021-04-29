#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("DataLogger"));
    w.show();
    return QApplication::exec();
}
