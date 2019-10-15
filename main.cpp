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

	return a.exec();
}
