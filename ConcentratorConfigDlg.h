#ifndef CONCENTRATORCONFIGDLG_H
#define CONCENTRATORCONFIGDLG_H

#include <QDialog>

namespace Ui {
class ConcentratorConfigDlg;
}

class ConcentratorConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConcentratorConfigDlg(QWidget *parent = 0);
    ~ConcentratorConfigDlg();

private:
    Ui::ConcentratorConfigDlg *ui;
};

#endif // CONCENTRATORCONFIGDLG_H
