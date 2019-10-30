#ifndef TERMINALGRAPHICSWIDGET_H
#define TERMINALGRAPHICSWIDGET_H

#include <QWidget>
#include "DatabaseProxy.h"

class lineTree
{
public:
    lineTree(proLine *l) {
        line = l;
    }

    int maxMonitorCount();
    int maxVerticalCount();

    void drawGraph(QPainter *p, int &x, int &y);
    void drawSimpleLine(QPainter *p, QRect rect, proMonitor *monitor);
    void drawConnectLine(QPainter *p, QRect rect);

    // line style, delete later
    void drawConnectorPath(QPainter *p, QRect rect);

    lineTree *parent;
    lineTree *preNode;
    proLine *line;
    QList<lineTree *> children;
};

class TerminalGraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TerminalGraphicsWidget(QWidget *parent = nullptr);

    void setType(bool isHistory);

    void init(proConcentrator *o);

    void updateValue();

protected:
    void paintEvent(QPaintEvent *event);

    void drawDefaultText(QPainter *p);

    void buildLine();
    void addChildLine(lineTree *node);

    void sortMonitor(proLine *line);

    QSize graphSize();
private:
    proConcentrator *_o;
    QList<lineTree *> _rootLine;
};

#endif // TERMINALGRAPHICSWIDGET_H
