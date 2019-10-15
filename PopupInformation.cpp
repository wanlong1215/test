#include "PopupInformation.h"
#include "ui_PopupInformation.h"

PopupInformation::PopupInformation(const QString &str, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupInformation)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->label->setText(str);
}

PopupInformation::~PopupInformation()
{
    delete ui;
}

void PopupInformation::on_pushButton_clicked()
{
    this->close();
}

void PopupInformation::closeEvent(QCloseEvent */*event*/)
{
    emit beforeClose(this);

    this->deleteLater();
}
