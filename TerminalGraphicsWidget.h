#ifndef TERMINALGRAPHICSWIDGET_H
#define TERMINALGRAPHICSWIDGET_H

#include <QWidget>
#include "DatabaseProxy.h"

class TerminalGraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TerminalGraphicsWidget(QWidget *parent = nullptr);

    void init(proConcentrator *o);

protected:
    void paintEvent(QPaintEvent *event);

    void drawDefaultText(QPainter *p);

    void drawSimpleLine(QPainter *p, QRect rect, proMonitor *monitor);
    void drawConnectLine(QPainter *p, QRect rect);

    // line style, delete later
    void drawConnectorPath(QPainter *p, QRect rect);

private:
    proConcentrator *_o;
};

#endif // TERMINALGRAPHICSWIDGET_H
