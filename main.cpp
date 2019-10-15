#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include "FarsiLoginDlg.h"
#include "Elec.h"
#include "DatabaseProxy.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QApplication a(argc, argv);
/*
    // build data
    auto com = new proCompany();
    com->name = QStringLiteral("公司");
    DatabaseProxy::instance().addCompany(com);
    auto subCom = new proSubCompany(com);
    subCom->name = QStringLiteral("分公司");
    DatabaseProxy::instance().addSubCompany(subCom, com->id);
    auto amso = new proAmso(subCom);
    amso->name = QStringLiteral("供电所");
    DatabaseProxy::instance().addAmso(amso, subCom->id);
    auto route = new proRoute(amso);
    route->name = QStringLiteral("线路");
    DatabaseProxy::instance().addRoute(route, amso->id);
    auto con = new proConcentrator(route);
    con->name = QStringLiteral("集中器");
    DatabaseProxy::instance().addConcentrator(con, route->id);
    for (int i = 0; i < 8; i++)
    {
        // add line
        auto l3 = new proLine(con);
        l3->name = QString("%1%2").arg(QStringLiteral("线段")).arg(i + 1);
        l3->preAddr = 1;
        if (i == 3 || i == 5)
        {
            l3->type = 2;
        }
        DatabaseProxy::instance().addLine(l3, con->id);

        for (int j = 0; j < 5; j++)
        {
            // add monitor
            auto m1 = new proMonitor(l3);
            m1->name = QString("L%1M%2").arg(i + 1).arg(j + 1);
            DatabaseProxy::instance().addMonitor(m1, l3->id);

            for (int k = 0; k < 3; k++)
            {
                auto t1 = new proTerminal(m1);
                t1->highPressureValue = 5 + 0.01 * (qrand() % 100);
                DatabaseProxy::instance().addTerminal(t1, m1->id);
            }
        }
    }
*/

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
