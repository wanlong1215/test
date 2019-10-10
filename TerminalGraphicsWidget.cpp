#include "TerminalGraphicsWidget.h"
#include <QPainter>

#define Margin 50
#define MonitorSize QSize(10, 150)
#define ConnSize QSize(50, 150)

TerminalGraphicsWidget::TerminalGraphicsWidget(QWidget *parent) : QWidget(parent), _o(NULL)
{
}

void TerminalGraphicsWidget::init(proConcentrator *o)
{
    _o = o;
    if (NULL == _o || _o->lst.isEmpty())
    {
        this->resize(parentWidget()->size());
        this->move(0, 0);
    }
    else
    {
        int monitorNum = 0;
        int connectNum = 0;
        for (int i = 0; i < _o->lst.count(); i++)
        {
            proLine *line = _o->lst.at(i);
            if (line->type != 2)
            {
                monitorNum += line->lst.count();
                connectNum += line->lst.count() - 1;
            }
            else
            {
                connectNum++;
            }

            if (i != _o->lst.count() - 1)
            {
                connectNum++;
            }
        }

        this->resize(monitorNum * MonitorSize.width() + connectNum * ConnSize.width() + Margin * 2, parentWidget()->height());
        this->move(0, 0);
    }

    update();
}

void TerminalGraphicsWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (NULL == _o || _o->lst.isEmpty())
    {
        drawDefaultText(&painter);
    }
    else
    {
        int totalWidth = Margin;
        QList<proLine *> lst = _o->getSortLine();
        for (int i = 0; i < lst.count(); i++)
        {
            proLine *line = lst.at(i);

            // 高压或者低压
            if (line->type != 2)
            {
                for (int j = 0; j < line->lst.count(); j++)
                {
                    proMonitor *monitor = line->lst.at(j);
                    if (!monitor->lst.isEmpty())
                    {
                        QRect rect(totalWidth, 30, MonitorSize.width(), MonitorSize.height());
                        totalWidth += MonitorSize.width();

                        drawSimpleLine(&painter, rect, monitor);
                    }

                    // connect line
                    if (j != line->lst.count() - 1)
                    {
                        QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
                        totalWidth += ConnSize.width();
                        drawConnectLine(&painter, rect);
                    }
                }
            }
            else
            {
                QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
                totalWidth += ConnSize.width();

                drawConnectorPath(&painter, rect);
            }

            // connect line
            if (i != lst.count() - 1)
            {
                QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
                totalWidth += ConnSize.width();
                drawConnectLine(&painter, rect);
            }
        }
    }
}

void TerminalGraphicsWidget::drawConnectorPath(QPainter *p, QRect rect)
{
    QPainterPath path;

    path.addEllipse(QRectF(rect.left(), rect.top() + rect.height() / 2, rect.width() * 2 / 3, rect.height() * 0.4));
    path.addEllipse(QRectF(rect.left() + rect.width() * 1 / 3, rect.top() + rect.height() / 2, rect.width() * 2 / 3, rect.height() * 0.4));

    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::transparent));
    p->drawPath(path);
}

void TerminalGraphicsWidget::drawDefaultText(QPainter *p)
{
    QFont font;

    font.setFamily("微软雅黑");
    font.setPixelSize(24);

    p->setPen(QPen(Qt::red));
    p->setBrush(QBrush(Qt::red));
    p->drawText(this->rect(), Qt::AlignCenter, QStringLiteral("当前集中器下暂无数据信息"));
}

void TerminalGraphicsWidget::drawSimpleLine(QPainter *p, QRect rect, proMonitor *monitor)
{
    // draw point
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::transparent));

    p->drawEllipse(QPoint(rect.center().x(), rect.top() + rect.height() * 0.7), 5, 5);

    p->setBrush(QBrush(Qt::black));
    p->drawLine(QPoint(rect.center().x() - 3, rect.top() + rect.height() * 0.7 - 3), QPoint(rect.center().x() + 3, rect.top() + rect.height() * 0.7 + 3));
    p->drawLine(QPoint(rect.center().x() - 3, rect.top() + rect.height() * 0.7 + 3), QPoint(rect.center().x() + 3, rect.top() + rect.height() * 0.7 - 3));

    // draw text
    QFont font;

    font.setFamily("微软雅黑");
    font.setPixelSize(10);

    p->setPen(QPen(Qt::red));
    p->setBrush(QBrush(Qt::red));
    p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.2), monitor->pressureValueA());
    p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.4), monitor->pressureValueB());
    p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.6), monitor->pressureValueC());

    // draw title
    p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.85), monitor->name);
}

void TerminalGraphicsWidget::drawConnectLine(QPainter *p, QRect rect)
{
    // draw rect
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::black));

    p->drawLine(QPoint(rect.left(), rect.top() + rect.height() * 0.7), QPoint(rect.right(), rect.top() + rect.height() * 0.7));
}
