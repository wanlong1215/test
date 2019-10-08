#ifndef CONCENTRATOREDITDLG_H
#define CONCENTRATOREDITDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class ConcentratorEditDlg;
}

class ConcentratorEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConcentratorEditDlg(proConcentrator *o, int parentId, QWidget *parent = 0);
    ~ConcentratorEditDlg();

    proConcentrator *concentrator();

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    void init();

private:
    Ui::ConcentratorEditDlg *ui;

    proConcentrator *_o;
    int _parentId;
};

#endif // CONCENTRATOREDITDLG_H
