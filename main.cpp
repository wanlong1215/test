﻿
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include "FarsiLoginDlg.h"
#include "Elec.h"
#include "DatabaseProxy.h"

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

	QApplication a(argc, argv);

    // build data
    auto com = new proCompany();
    com->name = "公司";
    DatabaseProxy::instance().addCompany(com);
    auto subCom = new proSubCompany(com);
    subCom->name = "分公司";
    DatabaseProxy::instance().addSubCompany(subCom, com->id);
    auto amso = new proAmso(subCom);
    amso->name = "供电所";
    DatabaseProxy::instance().addAmso(amso, subCom->id);
    auto route = new proRoute(amso);
    route->name = "线路";
    DatabaseProxy::instance().addRoute(route, amso->id);
    auto con = new proConcentrator(route);
    con->name = "集中器";
    DatabaseProxy::instance().addConcentrator(con, route->id);
    for (int i = 0; i < 8; i++)
    {
        // add line
        auto l3 = new proLine(con);
        l3->name = QString("线段%1").arg(i + 1);
        l3->preAddr = 1;
        l3->iValueA = 10;
        l3->iValueB = 11;
        l3->iValueC = 18;
        if (i == 3 || i == 5)
        {
            l3->type = 2;
        }
        DatabaseProxy::instance().addLine(l3, con->id);

        for (int j = 0; j < 5; j++)
        {
            // add monitor
            auto m1 = new proMonitor(l3);
            m1->name = QString("监视器%1").arg(j + 1);
            DatabaseProxy::instance().addMonitor(m1, l3->id);

            for (int k = 0; k < 3; k++)
            {
                auto t1 = new proTerminal(m1);
                t1->highPressureValue = j * 5 + k;
                t1->lowPressureValue = 10 + k;
                DatabaseProxy::instance().addTerminal(t1, m1->id);
            }
        }
    }


    QString fileName = ":/resource/css/blue.css";
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

//    Elec w;
//    w.show();

	return a.exec();
}
