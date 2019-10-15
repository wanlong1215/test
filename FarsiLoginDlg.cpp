#include "FarsiLoginDlg.h"
#include "ui_FarsiLoginDlg.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include "DatabaseConfigDlg.h"
#include <QDir>
#include "DatabaseProxy.h"
#include "AppSession.h"

#define RootUser "root"
#define RootPassword "2006"

FarsiLoginDlg::FarsiLoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FarsiLoginDlg)
{
    ui->setupUi(this);

    initUsersFromXml();
}

FarsiLoginDlg::~FarsiLoginDlg()
{
    delete ui;
}

void FarsiLoginDlg::initUsersFromXml()
{
    QFile file(QDir::currentPath() + QString("/xml/loginRecord.xml"));

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

    // 循环添加用户
    for (int i = 0; i < docElem.childNodes().count(); i++)
    {
        QDomNode nodeUser = docElem.childNodes().at(i);
        if (!nodeUser.isElement())
        {
            continue;
        }

        QDomElement eleUser = nodeUser.toElement();
        QString qsUser = eleUser.attribute(QString("usr"));

        ui->leUser->setText(qsUser);
        break;
    }
}

void FarsiLoginDlg::updateUsersOfXml()
{
    QFile file(QDir::currentPath() + QString("/xml/loginRecord.xml"));

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

    QString qsUser = ui->leUser->text();

    // 删除已有用户
    for (int i = 0; i < docElem.childNodes().count(); i++)
    {
        QDomNode nodeUser = docElem.childNodes().at(i);
        if (!nodeUser.isElement())
        {
            continue;
        }

        QDomElement eleUser = nodeUser.toElement();
        QString usr = eleUser.attribute(QString("usr"));
        if (usr != qsUser)
        {
            continue;
        }

        // 找到则移动到第一个位置
        docElem.removeChild(nodeUser);

        break;
    }

    // 将用户插入到第一个位置
    QDomElement eleUser = doc.createElement(QString("User"));
    eleUser.setAttribute(QString("usr"), qsUser);
    eleUser.setAttribute(QString("pwd"), QString(""));

    docElem.insertBefore(eleUser, docElem.firstChild());

    // 写入文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}

void FarsiLoginDlg::getConnectInfo(QString &ip, QString &usr, QString &pwd)
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

    ip = docElem.attribute(QString("ip"));
    usr = docElem.attribute(QString("usr"));
    pwd = docElem.attribute(QString("pwd"));
}

void FarsiLoginDlg::on_btnLogin_clicked()
{
    QString dbIP, dbUsr, dbPwd;
    getConnectInfo(dbIP, dbUsr, dbPwd);
    if (!DatabaseProxy::instance().connectDB(dbIP, dbUsr, dbPwd))
    {
        //
    }
    /*QString usr = ui->leUser->text();
    QString pwd = ui->lePwd->text();

    if (usr.isEmpty())
    {
        QMessageBox::warning(this,
            QStringLiteral("警告"),
            QStringLiteral("用户名不可以为空!"),
            QMessageBox::Ok);
        return;
    }

    QString dbIP, dbUsr, dbPwd;
    getConnectInfo(dbIP, dbUsr, dbPwd);
    if (!DatabaseProxy::instance().connectDB(dbIP, dbUsr, dbPwd))
    {
        QMessageBox::warning(this,
            QStringLiteral("警告"),
            QStringLiteral("数据库连接失败，请设置数据库连接信息!"),
            QMessageBox::Ok);
        return;
    }

    // 超级管理员
    if (usr == RootUser && pwd == RootPassword)
    {
        AppSession::instance().user.id = 0;
        AppSession::instance().user.level = 0;
        accept();
        return;
    }

    int id = DatabaseProxy::instance().userId(usr, pwd);
    if (-1 == id)
    {
        QMessageBox::warning(this,
            QStringLiteral("警告"),
            QStringLiteral("数据库内没有当前用户，请重新输入!"),
            QMessageBox::Ok);
        return;
    }

    AppSession::instance().user.id = id;
    AppSession::instance().user.level = DatabaseProxy::instance().userLevel(id);

    updateUsersOfXml();*/

	accept();
}

void FarsiLoginDlg::on_btnSet_clicked()
{
    DatabaseConfigDlg *dlg = new DatabaseConfigDlg(this);

    dlg->exec();
}
