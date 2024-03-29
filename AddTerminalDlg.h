﻿#ifndef AddTerminalDlg_H
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
    explicit AddTerminalDlg(int parentId, QWidget *parent = 0);
    ~AddTerminalDlg();

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

    int _parentId;
    proTerminal *_o1;
    proTerminal *_o2;
    proTerminal *_o3;
};

#endif // AddTerminalDlg_H
