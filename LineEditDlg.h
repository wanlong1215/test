#ifndef LINEEDITDLG_H
#define LINEEDITDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class LineEditDlg;
}

class LineEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LineEditDlg(proLine *o, int parentId, QWidget *parent = 0);
    ~LineEditDlg();

    proLine *line();

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    void init();

private:
    Ui::LineEditDlg *ui;

    int _parentId;
    proLine *_o;
    QMap<int, QString> _map;
};

#endif // LINEEDITDLG_H
