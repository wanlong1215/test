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

private slots:
    void on_btnTest_clicked();

    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    void initFromXml();
    void updateXml();

private:
    Ui::DatabaseConfigDlg *ui;

    QString _ip;
    QString _usr;
    QString _pwd;
};

#endif // DATABASECONFIGDLG_H
