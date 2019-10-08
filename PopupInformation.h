#ifndef POPUPINFORMATION_H
#define POPUPINFORMATION_H

#include <QWidget>

namespace Ui {
class PopupInformation;
}

class PopupInformation : public QWidget
{
    Q_OBJECT

public:
    explicit PopupInformation(QWidget *parent = 0);
    ~PopupInformation();

signals:
    void beforeClose(QWidget *wgt);

private slots:
    void on_pushButton_clicked();

private:
    void closeEvent(QCloseEvent *event);

private:
    Ui::PopupInformation *ui;
};

#endif // POPUPINFORMATION_H
