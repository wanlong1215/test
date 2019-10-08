#ifndef USERCONFIGDLG_H
#define USERCONFIGDLG_H

#include <QDialog>

namespace Ui {
class UserConfigDlg;
}

class UserConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserConfigDlg(QWidget *parent = 0);
    ~UserConfigDlg();

private:
    Ui::UserConfigDlg *ui;
};

#endif // USERCONFIGDLG_H
