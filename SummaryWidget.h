#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QWidget>
#include <QMap>
#include <QButtonGroup>

namespace Ui {
class SummaryWidget;
}

class proConcentrator;
class SummaryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryWidget(QWidget *parent = 0);
    ~SummaryWidget();

    void init();

private slots:
    void onRadioButtonClicked(int id);
    void onHistoryQuery();
    void onRealtimeQuery();
    void onHistoryQueryExec();
    void onRealtimeQueryExec();

    void onTimeout();

    void on_btnReadRealtime_clicked();
    void on_btnStopRead_clicked();
    void on_btnExport_clicked();
    void onSubCompayChanged();

private:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::SummaryWidget *ui;

    proConcentrator *_currentConcentrator;
    QTimer *_timer;
    QMap<QString, QStringList> _map; // subCompany concentrator
    QButtonGroup *_groupButton;
};

#endif // SUMMARYWIDGET_H
