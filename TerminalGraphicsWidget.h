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
    void drawGraphicsPath(QPainter *p, QRect rect);
    void drawTextPath(QPainter *p, QRect rect, const QString &s1, const QString &s2, const QString &s3, const QString &s4, const QString &s5, const QString &s6, const QString &s7);

private:
    proConcentrator *_o;
};

#endif // TERMINALGRAPHICSWIDGET_H
