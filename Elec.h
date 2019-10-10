#ifndef ELEC_H
#define ELEC_H

#include <QMainWindow>
#include "ui_Elec.h"

class Elec : public QMainWindow
{
	Q_OBJECT

public:
    Elec(QWidget *parent = 0);
	~Elec();

private slots:
    void on_btnSummary_clicked();

    void on_btnConfigure_clicked();

    void on_btnDatabase_clicked();

    void on_benServer_clicked();

    void on_btnWorker_clicked();

    void on_btnUser_clicked();

    void on_btnQuit_clicked();

    void on_pushButton_clicked();

    void onRemovePopupWidget(QWidget *obj);

    void on_btnAlarm_clicked();

private:
    void updateCheckStatus(QPushButton *btn);

private:
	Ui::ElecClass ui;

    QList<QWidget *> _lstPopupWidget;
    QList<QPushButton *> _lstMenuButton;
};

#endif // ELEC_H
