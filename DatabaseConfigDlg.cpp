#include "DatabaseConfigDlg.h"
#include "ui_DatabaseConfigDlg.h"
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QTextStream>
#include <QMessageBox>
#include "DatabaseProxy.h"

DatabaseConfigDlg::DatabaseConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseConfigDlg)
{
    ui->setupUi(this);

    initFromXml();
}

DatabaseConfigDlg::~DatabaseConfigDlg()
{
    delete ui;
}

void DatabaseConfigDlg::initFromXml()
{
    QFile file(QDir::currentPath() + QString("/xml/databaseRecord.xml"));

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    // 将文件内容读到QDomDocument中
    QDomDocument doc;
    bool bLoadFile = doc.setContent(&file);
    file.close();

    if (!bLoadFile)
    {
        return;
    }

    QDomElement docElem = doc.documentElement();

    _ip = docElem.attribute(QString("ip"));
    _usr = docElem.attribute(QString("usr"));
    _pwd = docElem.attribute(QString("pwd"));

    ui->leIP->setText(_ip);
    ui->leUser->setText(_usr);
    ui->lePassword->setText(_pwd);
}

void DatabaseConfigDlg::updateXml()
{
    QFile file(QDir::currentPath() + QString("/xml/databaseRecord.xml"));

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    // 将文件内容读到QDomDocument中
    QDomDocument doc;
    bool bLoadFile = doc.setContent(&file);
    file.close();

    if (!bLoadFile)
    {
        return;
    }

    // 二级链表
    QDomElement docElem = doc.documentElement();

    docElem.setAttribute("ip", ui->leIP->text());
    docElem.setAttribute("usr", ui->leUser->text());
    docElem.setAttribute("pwd", ui->lePassword->text());

    // 写入文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}

void DatabaseConfigDlg::on_btnTest_clicked()
{
    if (ui->leIP->text().isEmpty() || ui->leUser->text().isEmpty() || ui->lePassword->text().isEmpty())
    {
        QMessageBox::warning(this,
            QStringLiteral("警告"),
            QStringLiteral("连接信息不可以为空，请检查后进行测试!"),
            QMessageBox::Ok);
        return;
    }

    QMessageBox::warning(this,
                         QStringLiteral("警告"),
                         DatabaseProxy::instance().connectDB(ui->leIP->text(), ui->leUser->text(), ui->lePassword->text()) ?
                             QStringLiteral("连接成功!") : QStringLiteral("连接失败!"),
                         QMessageBox::Ok);

    // need reconnect use old login info
    if (DatabaseProxy::instance().isDBConnected())
    {
        DatabaseProxy::instance().connectDB(_ip, _usr, _pwd);
    }
}

void DatabaseConfigDlg::on_btnOK_clicked()
{
    updateXml();
    accept();
}

void DatabaseConfigDlg::on_btnCancel_clicked()
{
    reject();
}
