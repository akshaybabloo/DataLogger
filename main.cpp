#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <qtextstream.h>

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

    QFile f(":style.qss");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication a(argc, argv);

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

//    QApplication::setStyle("Fusion");
    MainWindow w;
    w.setWindowTitle(QString("DataLogger"));
    w.setWindowIcon(QIcon(":/images/datalogger.png"));
    w.show();
    return QApplication::exec();
}
