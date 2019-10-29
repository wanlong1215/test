#include "TerminalGraphicsWidget.h"
#include <QPainter>

#define Margin 50
#define MonitorSize QSize(30, 150)
#define ConnSize QSize(50, 150)

TerminalGraphicsWidget::TerminalGraphicsWidget(QWidget *parent) : QWidget(parent), _o(NULL)
{
}

void TerminalGraphicsWidget::setType(bool isHistory)
{
    _isHistory = isHistory;
}

void TerminalGraphicsWidget::init(proConcentrator *o)
{
    _o = o;
    while (!_rootLine.isEmpty()) {
        delete _rootLine.takeFirst();
    }

    if (NULL == _o || _o->lst.isEmpty())
    {
        this->resize(parentWidget()->size());
        this->move(0, 0);
    }
    else
    {
        buildLine();
        this->resize(graphSize());
        this->move(0, 0);

//        int monitorNum = 0;
//        int connectNum = 0;
//        for (int i = 0; i < _o->lst.count(); i++)
//        {
//            proLine *line = _o->lst.at(i);
//            if (line->type != 2)
//            {
//                monitorNum += line->lst.count();
//                connectNum += line->lst.count() - 1;
//            }
//            else
//            {
//                connectNum++;
//            }

//            if (i != _o->lst.count() - 1)
//            {
//                connectNum++;
//            }
//        }

//        this->resize(monitorNum * MonitorSize.width() + connectNum * ConnSize.width() + Margin * 2, parentWidget()->height());
//        this->move(0, 0);
    }

    update();
}

void TerminalGraphicsWidget::updateValue()
{
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
        int totalHeight = Margin;
        foreach (auto node, _rootLine) {
            totalWidth = Margin;
            node->drawGraph(&painter, totalWidth, totalHeight);
//            auto line = node->line;
//            // 高压或者低压
//            if (line->type != 2)
//            {
//                for (int j = 0; j < line->lst.count(); j++)
//                {
//                    proMonitor *monitor = line->lst.at(j);
//                    if (!monitor->lst.isEmpty())
//                    {
//                        QRect rect(totalWidth, 30, MonitorSize.width(), MonitorSize.height());
//                        totalWidth += MonitorSize.width();

//                        drawSimpleLine(&painter, rect, monitor);
//                    }

//                    // connect line
//                    if (j != line->lst.count() - 1)
//                    {
//                        QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
//                        totalWidth += ConnSize.width();
//                        drawConnectLine(&painter, rect);
//                    }
//                }
//            }
//            else
//            {
//                QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
//                totalWidth += ConnSize.width();

//                drawConnectorPath(&painter, rect);
//            }

        }

//        int totalWidth = Margin;
//        QList<proLine *> lst = _o->getSortLine();
//        for (int i = 0; i < lst.count(); i++)
//        {
//            proLine *line = lst.at(i);

//            // 高压或者低压
//            if (line->type != 2)
//            {
//                for (int j = 0; j < line->lst.count(); j++)
//                {
//                    proMonitor *monitor = line->lst.at(j);
//                    if (!monitor->lst.isEmpty())
//                    {
//                        QRect rect(totalWidth, 30, MonitorSize.width(), MonitorSize.height());
//                        totalWidth += MonitorSize.width();

//                        drawSimpleLine(&painter, rect, monitor);
//                    }

//                    // connect line
//                    if (j != line->lst.count() - 1)
//                    {
//                        QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
//                        totalWidth += ConnSize.width();
//                        drawConnectLine(&painter, rect);
//                    }
//                }
//            }
//            else
//            {
//                QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
//                totalWidth += ConnSize.width();

//                drawConnectorPath(&painter, rect);
//            }

//            // connect line
//            if (i != lst.count() - 1)
//            {
//                QRect rect(totalWidth, 30, ConnSize.width(), ConnSize.height());
//                totalWidth += ConnSize.width();
//                drawConnectLine(&painter, rect);
//            }
//        }
    }
}

void lineTree::drawConnectorPath(QPainter *p, QRect rect)
{
    QPainterPath path;

    double width = rect.width() * 2.0 / 3;
    path.addEllipse(QRectF(rect.left(), rect.top() + rect.height() *0.7 - width/2, width, width));
    path.addEllipse(QRectF(rect.left() + width / 2, rect.top() + rect.height() *0.7 - width/2, width, width));

    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::transparent));
    p->drawPath(path);
}

void TerminalGraphicsWidget::buildLine()
{
    foreach (auto line, _o->lst) {
        if (line->preAddr == QString::number(-1) || line->preAddr == QString::number(0)) {
            auto lineNode = new lineTree(line);
            addChildLine(lineNode);
            _rootLine.append(lineNode);
        }
    }
}

void TerminalGraphicsWidget::addChildLine(lineTree *node)
{
    foreach (auto line, _o->lst) {
        if (line->preAddr == QString::number(node->line->id)) {
            auto child = new lineTree(line);
            addChildLine(child);
            node->children.append(child);
        }
    }
}

QSize TerminalGraphicsWidget::graphSize()
{
    int monCount = 0;
    int verCount = 0;
    foreach(auto node, _rootLine) {
        monCount = qMax(node->maxMonitorCount(), monCount);
        verCount += node->maxVerticalCount();
    }

    int width = monCount * MonitorSize.width() + (monCount-1) * ConnSize.width() + 2 * Margin;
    int height = verCount * MonitorSize.height() + 2 * Margin;

    return QSize(width, height);
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

void lineTree::drawSimpleLine(QPainter *p, QRect rect, proMonitor *monitor)
{
    // draw point
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::transparent));

    p->drawEllipse(QPoint(rect.center().x(), rect.top() + rect.height() * 0.6), 15, 15);

    p->setBrush(QBrush(Qt::black));
    int offset = 10;
    p->drawLine(QPoint(rect.center().x() - offset, rect.top() + rect.height() * 0.6 - offset), QPoint(rect.center().x() + offset, rect.top() + rect.height() * 0.6 + offset));
    p->drawLine(QPoint(rect.center().x() - offset, rect.top() + rect.height() * 0.6 + offset), QPoint(rect.center().x() + offset, rect.top() + rect.height() * 0.6 - offset));

    // draw text
    QFont font;

    font.setFamily("微软雅黑");
    font.setPixelSize(10);

    p->setPen(QPen(Qt::red));
    p->setBrush(QBrush(Qt::red));
    //if (_isHistory) {
    if (true) {
        p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.2), monitor->pressureValueA());
        p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.3), monitor->pressureValueB());
        p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.4), monitor->pressureValueC());
    } else {
        p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.2), monitor->rtva);
        p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.3), monitor->rtvb);
        p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.4), monitor->rtvc);
    }

    // draw title
    p->drawText(QPoint(rect.left(), rect.top() + rect.height() * 0.8), monitor->name);
}

void lineTree::drawConnectLine(QPainter *p, QRect rect)
{
    // draw rect
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::black));

    p->drawLine(QPoint(rect.left(), rect.top() + rect.height() * 0.6), QPoint(rect.right(), rect.top() + rect.height() * 0.6));
}

int lineTree::maxMonitorCount()
{
    int count = (line->type == 2) ? 1 : line->lst.count();

    if (children.isEmpty()) {
        //
    } else {
        int maxCount = 0;
        foreach (auto l, children) {
            maxCount = qMax(maxCount, l->maxMonitorCount());
        }
        count += maxCount;
    }

    return count;
}

int lineTree::maxVerticalCount()
{
    int count = 0;
    if (line->type == 2) {
        count = 1;
    } else {
        if (line->lst.isEmpty()) {
            count = 0;
        } else {
            count = 1;
        }
    }

    count = qMax(children.count(), count);

    foreach (auto l, children) {
        int maxCount = qMax(1, l->maxVerticalCount());
        if (maxCount > 1) {
            count += maxCount-1;
        }
    }

    return count;
}

void lineTree::drawGraph(QPainter *p, int &totalWidth, int &totalHeight)
{
    if (line->type != 2)
    {
        for (int j = 0; j < line->lst.count(); j++)
        {
            proMonitor *monitor = line->lst.at(j);
            if (!monitor->lst.isEmpty())
            {
                QRect rect(totalWidth, totalHeight, MonitorSize.width(), MonitorSize.height());
                totalWidth += MonitorSize.width();

                drawSimpleLine(p, rect, monitor);
            }

            // connect line
            if (j != line->lst.count() - 1)
            {
                QRect rect(totalWidth, totalHeight, ConnSize.width(), ConnSize.height());
                totalWidth += ConnSize.width();

                drawConnectLine(p, rect);
            }
        }
    }
    else
    {
        QRect rect(totalWidth, totalHeight, ConnSize.width(), ConnSize.height());
        totalWidth += ConnSize.width();
        totalHeight += ConnSize.height();

        drawConnectorPath(p, rect);
    }

    if (!children.isEmpty()) {
        QRect rect(totalWidth, totalHeight, ConnSize.width(), ConnSize.height());
        totalWidth += ConnSize.width();

        drawConnectLine(p, rect);
    }

    foreach (auto child, children) {
        child->drawGraph(p, totalWidth, totalHeight);

        if (!child->line->lst.isEmpty()) {
            totalHeight += MonitorSize.height();
        }
    }
}

