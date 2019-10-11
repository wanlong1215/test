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
    QFile file(QDir::currentPath() + QString("/loginRecord.xml"));

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
    QFile file(QDir::currentPath() + QString("/loginRecord.xml"));

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

void FarsiLoginDlg::on_btnLogin_clicked()
{
    QString usr = ui->leUser->text();
    QString pwd = ui->lePwd->text();

    if (usr.isEmpty())
    {
        QMessageBox::warning(this,
            tr("警告"),
            tr("用户名不可以为空!"),
            QMessageBox::Ok);
        return;
    }

    if (!DatabaseProxy::instance().connectDB("ip", "usr", "pwd"))
    {
        QMessageBox::warning(this,
            tr("警告"),
            tr("数据库连接失败，请设置数据库连接信息!"),
            QMessageBox::Ok);
        return;
    }

    int id = DatabaseProxy::instance().userId(usr, pwd);
    if (-1 == id)
    {
        QMessageBox::warning(this,
            tr("警告"),
            tr("数据库内没有当前用户，请重新输入!"),
            QMessageBox::Ok);
        return;
    }

    AppSession::instance().user.id = id;
    AppSession::instance().user.level = DatabaseProxy::instance().userLevel(id);

    updateUsersOfXml();

	accept();

	/*
     * // 是否连接服务器
     * BCCommon::g_bConnectWithServer = false;
     * if ( !BCCommon::g_bConnectWithServer ) {
     *  // 加载系统人员权限数据
     *  BCLocalServer *pLocalServer = BCLocalServer::Application();
     *  QPoint pt = pLocalServer->Authenticate(qsUser, qsPwd);
     *
     *  SetServerRes(pt.x(), pt.y());
     * }*/
}


void FarsiLoginDlg::SetServerRes(int userid, int level)
{
/*    if (-1 == userid) {
 *      QString qsWarning = BCCommon::g_bConnectWithServer ? tr("用户名或密码错误，如确认登录信息无误请检查服务器配置是否正确!") : tr("用户名或密码错误!");
 *      QMessageBox::warning(this,
 *                           tr("警告"),
 *                           qsWarning,
 *                           QMessageBox::Ok);
 *  } else if (-2 == userid) {
 *      QString qsWarning = BCCommon::g_bConnectWithServer ? tr("当前用户已经在其他设备上登录，不可重复登录!") : tr("用户名或密码错误!");
 *      QMessageBox::warning(this,
 *                           tr("警告"),
 *                           qsWarning,
 *                           QMessageBox::Ok);
 *  } else {
 *      MainWindow *pApplication = BCCommon::Application();
 *
 *      BCSUser *pUser = new BCSUser;
 *      pUser->id = userid;
 *      pUser->loginName = ui->leUser->text();
 *      pUser->password = ui->lePwd->text();
 *      pUser->level = level;
 *      pApplication->SetCurrentUser( pUser );
 *
 *      // 登录成功修改xml文件，不存储root用户
 *      if (0 != level)
 *          updateUsersOfXml();
 *
 *      accept();
 *  }*/
}

void FarsiLoginDlg::on_btnSet_clicked()
{
    DatabaseConfigDlg *dlg = new DatabaseConfigDlg(this);

    dlg->exec();
}
