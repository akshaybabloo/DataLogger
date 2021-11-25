#include "aboutwidget.h"
#include "ui_aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AboutWidget) {
    ui->setupUi(this);

    QPixmap logo(":/images/datalogger.png");
    ui->logo->setPixmap(logo.scaledToHeight(100, Qt::SmoothTransformation));

    ui->version->setText(tr("v2.0.0"));

    auto datetime = QStringLiteral(__DATE__) + QStringLiteral(" ") + QStringLiteral(__TIME__);
    ui->builtWith->setText(tr("Built with Qt v%1 on %2").arg(QT_VERSION_STR, datetime));

    // TODO: add details here
    QString details = R"(Provided under MIT license

This application uses the following external libraries:

1. Eigen - MPL 2.0 - https://eigen.tuxfamily.org/
)";
    ui->detailsEdit->setPlainText(details);
}

AboutWidget::~AboutWidget() {
    delete ui;
}
