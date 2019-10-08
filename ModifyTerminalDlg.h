#ifndef MODIFYTERMINALDLG_H
#define MODIFYTERMINALDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class ModifyTerminalDlg;
}

class ModifyTerminalDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyTerminalDlg(proTerminal *o, QWidget *parent = 0);
    ~ModifyTerminalDlg();

    proTerminal *terminal();

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    void init();

private:
    Ui::ModifyTerminalDlg *ui;

    proTerminal *_o;
};

#endif // MODIFYTERMINALDLG_H
