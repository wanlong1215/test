#ifndef BASICEDITDLG_H
#define BASICEDITDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class BasicEditDlg;
}

class BasicEditDlg : public QDialog
{
    Q_OBJECT

public:
    BasicEditDlg(int addType, int pid, QWidget *parent = 0);// addType 1:添加公司 2:添加分公司 3：添加供电所 4：添加线路
    BasicEditDlg(proCompany *o, QWidget *parent = 0);       // 修改公司
    BasicEditDlg(proSubCompany *o, QWidget *parent = 0);    // 修改分公司
    BasicEditDlg(proAmso *o, QWidget *parent = 0);          // 修改供电所
    BasicEditDlg(proRoute *o, QWidget *parent = 0);         // 修改线路
    ~BasicEditDlg();

    proCompany *company();
    proSubCompany *subCompany();
    proAmso *amso();
    proRoute *route();

    QString name();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::BasicEditDlg *ui;

    int _type;
    int _parentId;
    proCompany *_o1;
    proSubCompany *_o2;
    proAmso *_o3;
    proRoute *_o4;
};

#endif // BASICEDITDLG_H
