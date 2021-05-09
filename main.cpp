#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QString dateTime = QDateTime::currentDateTime().toString();

    switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "%s - Debug: %s (%s:%u, %s)\n", dateTime.toLocal8Bit().data(), localMsg.constData(),
                    context.file, context.line, context.function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "%s - Info: %s (%s:%u, %s)\n", dateTime.toLocal8Bit().data(), localMsg.constData(),
                    context.file, context.line, context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "%s - Warning: %s (%s:%u, %s)\n", dateTime.toLocal8Bit().data(), localMsg.constData(),
                    context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "%s - Critical: %s (%s:%u, %s)\n", dateTime.toLocal8Bit().data(), localMsg.constData(),
                    context.file, context.line, context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "%s - Fatal: %s (%s:%u, %s)\n", dateTime.toLocal8Bit().data(), localMsg.constData(),
                    context.file, context.line, context.function);
            abort();
    }
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication::setOrganizationName("Gollahalli");
    QCoreApplication::setOrganizationDomain("gollahalli.com");
    QCoreApplication::setApplicationName("DataLogger");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication a(argc, argv);

    QSettings settings;
    qInfo() << settings.fileName();
    auto theme = settings.value("theme", "dark").toString();
    qInfo() << theme;

    if (theme == "dark") {
        QFile darkTheme(":style-dark.qss");
        darkTheme.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&darkTheme);
        a.setStyleSheet(ts.readAll());
    } else if (theme == "light") {
        QFile light(":style-light.qss");
        light.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&light);
        a.setStyleSheet(ts.readAll());
    } else {
        QFile light(":style-light.qss");
        light.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&light);
        a.setStyleSheet(ts.readAll());
    }

//    QApplication::setStyle("Fusion");
    MainWindow w;
    w.setWindowTitle(QString("DataLogger"));
    w.setWindowIcon(QIcon(":/images/datalogger.png"));
    w.show();
    return QApplication::exec();
}
