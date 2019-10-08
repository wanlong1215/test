#ifndef DATABASECONFIGDLG_H
#define DATABASECONFIGDLG_H

#include <QDialog>

namespace Ui {
class DatabaseConfigDlg;
}

class DatabaseConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseConfigDlg(QWidget *parent = 0);
    ~DatabaseConfigDlg();

private:
    Ui::DatabaseConfigDlg *ui;
};

#endif // DATABASECONFIGDLG_H
