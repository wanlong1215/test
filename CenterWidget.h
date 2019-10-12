#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "DatabaseProxy.h"

namespace Ui {
class CenterWidget;
}

class CenterWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CenterWidget(QWidget *parent = 0);
	~CenterWidget();

	void init();

protected:
	bool eventFilter(QObject *obj, QEvent *e);

    void addItem(QTreeWidgetItem * item);
    void modifyItem(QTreeWidgetItem * item);
    void delItem(QTreeWidgetItem * item);

    void showTerminal(proLine *o);

private:
	Ui::CenterWidget *ui;
};

#endif // CENTERWIDGET_H
