#ifndef FARSILOGINDLG_H
#define FARSILOGINDLG_H

#include <QDialog>

namespace Ui {
class FarsiLoginDlg;
}

class FarsiLoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FarsiLoginDlg(QWidget *parent = 0);
    ~FarsiLoginDlg();

private slots:
    void on_btnLogin_clicked();
    void on_btnSet_clicked();

private:
    // 从xml中读取已经登录过的用户
    void initUsersFromXml();

    // 更新xml中的用户信息
    void updateUsersOfXml();

    void getConnectInfo(QString &ip, QString &usr, QString &pwd);

private:
    Ui::FarsiLoginDlg *ui;
};

#endif // FARSILOGINDLG_H
