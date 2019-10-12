#ifndef USERCONFIGDLG_H
#define USERCONFIGDLG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class UserConfigDlg;
}

class UserConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserConfigDlg(QWidget *parent = 0);
    ~UserConfigDlg();

private slots:
    void onCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_removeBut_clicked();

    void on_addBut_clicked();

    void on_m_pModifyBtn_clicked();

private:
    Ui::UserConfigDlg *ui;

    void Init();

    // 记录级别和级别名称的关系
    QMap<int, QString> _map;
};

#endif // USERCONFIGDLG_H
