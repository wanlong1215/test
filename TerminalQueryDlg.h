#ifndef TERMINALQUERYDLG_H
#define TERMINALQUERYDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class TerminalQueryDlg;
}

class TerminalQueryDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TerminalQueryDlg(QWidget *parent = 0);
    ~TerminalQueryDlg();

private slots:
    void onSubCompayChanged();
    void onAsmoChanged();
    void onLineChanged();
    void onWorkerChanged();
    void onQuery();

    void on_btnExport_clicked();

private:
    void init();

    int getTerminalCount();

private:
    Ui::TerminalQueryDlg *ui;

    QList<proCompany *> _company; // 临时记录，增加速度
};

#endif // TERMINALQUERYDLG_H
