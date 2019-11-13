#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QWidget>
#include <QMap>
#include <QButtonGroup>
#include <QLabel>
#include "DatabaseProxy.h"

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

signals:
    void sigLoadingControl(bool power);

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

    void onLoadingControl(bool power);

    void on_btnSelectAll_clicked();

    void on_btnUnSelectAll_clicked();

    void on_btnPrePage_clicked();

    void on_btnNextPage_clicked();

private:
    bool eventFilter(QObject *obj, QEvent *e);
    void resizeEvent(QResizeEvent *e);

    void refreshHistoryData();
private:
    Ui::SummaryWidget *ui;

    proConcentrator *_currentConcentrator;
    QTimer *_timer;
    QMap<QString, QStringList> _map; // subCompany concentrator
    QButtonGroup *_groupButton;

    bool _isRunning{false};
    QLabel *_historyLoading{nullptr};

    QList<showData> _historyDatas;

    // history data
    int _pageNumber;
    int _pageCount;
};

#endif // SUMMARYWIDGET_H
