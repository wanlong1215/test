﻿#ifndef SERVERCONFIGDLG_H
#define SERVERCONFIGDLG_H

#include <QDialog>

namespace Ui {
class ServerConfigDlg;
}

class ServerConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ServerConfigDlg(QWidget *parent = 0);
    ~ServerConfigDlg();

private:
    Ui::ServerConfigDlg *ui;
};

#endif // SERVERCONFIGDLG_H
