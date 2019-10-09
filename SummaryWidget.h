#ifndef SUMMARYWIDGET_H
#define SUMMARYWIDGET_H

#include <QWidget>

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
    void onAutoQueryToggled(bool b);
    void onQuickQueryToggled(bool b);
    void onAbsoluteQueryToggled(bool b);
    void onHistoryQuery();
    void onRealtimeQuery();

private:
    bool eventFilter(QObject *obj, QEvent *e);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::SummaryWidget *ui;

    proConcentrator *_currentConcentrator;
};

#endif // SUMMARYWIDGET_H
