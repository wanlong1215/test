#ifndef AddTerminalDlg_H
#define AddTerminalDlg_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class AddTerminalDlg;
}

class AddTerminalDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddTerminalDlg(QList<proCompany *> lst, int parentId, QWidget *parent = 0);
    ~AddTerminalDlg();

    proMonitor *monitor();
    proTerminal *terminal1();
    proTerminal *terminal2();
    proTerminal *terminal3();

private slots:
    void on_btnCancel_clicked();
    void on_btnOK_clicked();

private:
    void init();

private:
    Ui::AddTerminalDlg *ui;

    QList<proCompany *> _com;
    int _parentId;
    proMonitor *_monitor;
    proTerminal *_o1;
    proTerminal *_o2;
    proTerminal *_o3;
    QMap<int, QString> _map;
};

#endif // AddTerminalDlg_H
