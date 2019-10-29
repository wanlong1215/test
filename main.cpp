#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include "FarsiLoginDlg.h"
#include "Elec.h"
#include "DatabaseProxy.h"
#include <QDebug>
#include <QMutex>

void MessageOutput(QtMsgType type,const QMessageLogContext& context,const QString& msg)
{
    QString txtMessage;
    QMutex mutex;
    //加锁
    mutex.lock();
    //设置log输出格式
    txtMessage += QString("[%1][%2][%3]")
            .arg(QTime::currentTime().toString("hh:mm:ss"))
            .arg(context.file)
            .arg(context.function);
    switch (type) {
    case QtDebugMsg:
        txtMessage += QString("[Debug] %1").arg(msg);
        break;
    case QtWarningMsg:
        //txtMessage += QString("[Warning] %1").arg(msg);
        break;
    case QtCriticalMsg:
        txtMessage += QString("[Critical] %1").arg(msg);
        break;
    case QtFatalMsg:
        txtMessage += QString("[Fatal] %1").arg(msg);
        abort();
        break;
    default:
        txtMessage += QString("[UnKnown] %1").arg(msg);
        break;
    }
    txtMessage += QString(",{%1}").arg(context.line);
    txtMessage += QString("\r\n");

    QFile file("debug.log");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&file);
        out<<txtMessage;
    }
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}
#include <QDateTime>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    qInstallMessageHandler(MessageOutput);
	QApplication a(argc, argv);


//    QList<showData> lst;
//    DatabaseProxy::instance().connectDB("127.0.0.1", "Administrator", "123");
//    DatabaseProxy::instance().getOrganizations();
//    auto beginTime = QDateTime::currentDateTime();
//    qDebug() << "begin query history data";
//    DatabaseProxy::instance().historyDataByTime(lst, 2);
//    qDebug() << QString("end query history data, cost time: %1 ms").arg(beginTime.msecsTo(QDateTime::currentDateTime()));

//    return a.exec();

    //QString fileName = ":/resource/css/blue.css";
    QString fileName = ":/resource/css/flatwhite.css";
    //QString fileName = ":/resource/css/lightblue.css";
    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly))
        {
            QString str = file.readAll();

            QString paletteColor = str.mid(20, 7);
            qApp->setPalette(QPalette(QColor(paletteColor)));
            qApp->setStyleSheet(str);
        }
    }

    FarsiLoginDlg dlg;
    dlg.showMaximized();
    if (QDialog::Accepted == dlg.exec())
    {
        Elec *w = new Elec();
        w->showMaximized();
    }
    else
    {
        return 0;
    }

	return a.exec();
}
