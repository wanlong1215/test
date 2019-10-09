#include "TerminalGraphicsWidget.h"
#include <QPainter>

#define LeftMargin 30
#define LineSize QSize(50, 150)
#define ConnSize QSize(30, 150)

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
        for (int i = 0; i < _o->lst.count(); i++)
        {
            proLine *line = _o->lst.at(i);
            if (line->type != 2)
            {
                monitorNum += line->lst.count();
            }
            else
            {
                monitorNum += 1;
            }
        }

        this->resize(monitorNum * (LineSize.width() + ConnSize.width()) - ConnSize.width() + LeftMargin * 2, parentWidget()->height());
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
        int totalWidth = LeftMargin;
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
                        QRect rect(totalWidth, 30, LineSize.width(), LineSize.height());
                        totalWidth += LineSize.width();

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

    path.addEllipse(QRectF(rect.left(), rect.top(), rect.width() * 2 / 3, rect.height()));
    path.addEllipse(QRectF(rect.left() + rect.width() * 1 / 3, rect.top(), rect.width() * 2 / 3, rect.height()));
//    path.moveTo(rect.topLeft());
//    path.lineTo(rect.topRight());
//    path.moveTo(rect.left(), rect.top() + rect.height() / 2);
//    path.lineTo(rect.right(), rect.top() + rect.height() / 2);

    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::transparent));
    p->drawPath(path);
}

void TerminalGraphicsWidget::drawGraphicsPath(QPainter *p, QRect rect)
{
    QPainterPath pathGraphics;

    // 高压线
    pathGraphics.moveTo(rect.topLeft());
    pathGraphics.lineTo(rect.topRight());
    pathGraphics.moveTo(rect.left(), rect.top() + rect.height() / 4);
    pathGraphics.lineTo(rect.right(), rect.top() + rect.height() / 4);
    pathGraphics.moveTo(rect.left(), rect.top() + rect.height() / 2);
    pathGraphics.lineTo(rect.right(), rect.top() + rect.height() / 2);
    pathGraphics.moveTo(rect.left() + rect.width() / 2, rect.top() + rect.height() / 4);
    pathGraphics.lineTo(rect.left() + rect.width() / 2, rect.bottom());
    pathGraphics.moveTo(rect.left() + rect.width() * 0.6, rect.top());
    pathGraphics.lineTo(rect.left() + rect.width() * 0.4, rect.top() + rect.height() / 2);

    // 6个节点
    pathGraphics.addEllipse(QPointF(rect.left() + rect.width() * 0.2, rect.top()), 2, 2);
    pathGraphics.addEllipse(QPointF(rect.left() + rect.width() * 0.8, rect.top()), 2, 2);
    pathGraphics.addEllipse(QPointF(rect.left() + rect.width() * 0.2, rect.top() + rect.height() / 4), 2, 2);
    pathGraphics.addEllipse(QPointF(rect.left() + rect.width() * 0.8, rect.top() + rect.height() / 4), 2, 2);
    pathGraphics.addEllipse(QPointF(rect.left() + rect.width() * 0.2, rect.top() + rect.height() / 2), 2, 2);
    pathGraphics.addEllipse(QPointF(rect.left() + rect.width() * 0.8, rect.top() + rect.height() / 2), 2, 2);

    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::black));
    p->drawPath(pathGraphics);
}

void TerminalGraphicsWidget::drawTextPath(QPainter *p, QRect rect, const QString &s1, const QString &s2, const QString &s3, const QString &s4, const QString &s5, const QString &s6, const QString &s7)
{
    QPainterPath pathText;

    QFont font;

    font.setFamily("微软雅黑");
    font.setPixelSize(10);
    int offsetX = 5;
    int offsetY = -5;
    pathText.addText(QPointF(rect.left() + rect.width() * 0.2 + offsetX, rect.top() + offsetY), font, s1);
    pathText.addText(QPointF(rect.left() + rect.width() * 0.8 + offsetX, rect.top() + offsetY), font, s2);
    pathText.addText(QPointF(rect.left() + rect.width() * 0.2 + offsetX, rect.top() + rect.height() / 4 + offsetY), font, s3);
    pathText.addText(QPointF(rect.left() + rect.width() * 0.8 + offsetX, rect.top() + rect.height() / 4 + offsetY), font, s4);
    pathText.addText(QPointF(rect.left() + rect.width() * 0.2 + offsetX, rect.top() + rect.height() / 2 + offsetY), font, s5);
    pathText.addText(QPointF(rect.left() + rect.width() * 0.8 + offsetX, rect.top() + rect.height() / 2 + offsetY), font, s6);
    pathText.addText(QPoint(rect.left() + rect.width() * 0.5 + offsetX, rect.bottom() + offsetY), font, s7);

    p->setPen(QPen(Qt::red));
    p->setBrush(QBrush(Qt::red));
    p->drawPath(pathText);
}

void TerminalGraphicsWidget::drawDefaultText(QPainter *p)
{
    QFont font;

    font.setFamily("微软雅黑");
    font.setPixelSize(20);

    p->setPen(QPen(Qt::red));
    p->setBrush(QBrush(Qt::red));
    p->drawText(this->rect(), Qt::AlignCenter, QString("当前集中器下暂无线段"));
}

void TerminalGraphicsWidget::drawSimpleLine(QPainter *p, QRect rect, proMonitor *monitor)
{
    // draw rect
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::transparent));
    //p->drawRect(rect);

    // draw 3 points
    p->setBrush(QBrush(Qt::black));
    p->drawEllipse(QPoint(rect.center().x(), rect.top() + rect.height() * 0.2), 2, 2);
    p->drawEllipse(rect.center(), 2, 2);
    p->drawEllipse(QPoint(rect.center().x(), rect.top() + rect.height() * 0.8), 2, 2);

    // draw text
    QFont font;

    font.setFamily("微软雅黑");
    font.setPixelSize(10);
    int offsetX = 5;
    int offsetY = -5;

    p->setPen(QPen(Qt::red));
    p->setBrush(QBrush(Qt::red));
    p->drawText(QPoint(rect.center().x() + offsetX, rect.top() + rect.height() * 0.2 + offsetY), monitor->pressureValueA());
    p->drawText(QPoint(rect.center().x() + offsetX, rect.center().y() + offsetY), monitor->pressureValueB());
    p->drawText(QPoint(rect.center().x() + offsetX, rect.top() + rect.height() * 0.8 + offsetY), monitor->pressureValueC());

    // draw title
    p->drawText(QPoint(rect.left() + offsetX, rect.top() + offsetY), monitor->name);
}

void TerminalGraphicsWidget::drawConnectLine(QPainter *p, QRect rect)
{
    // draw rect
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::black));

    p->drawLine(QPoint(rect.left(), rect.top() + rect.height() * 0.2), QPoint(rect.right(), rect.top() + rect.height() * 0.2));
    p->drawLine(QPoint(rect.left(), rect.center().y()), QPoint(rect.right(), rect.center().y()));
    p->drawLine(QPoint(rect.left(), rect.top() + rect.height() * 0.8), QPoint(rect.right(), rect.top() + rect.height() * 0.8));
}
