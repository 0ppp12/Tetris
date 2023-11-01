#ifndef NEXTAREA_H
#define NEXTAREA_H

#include "item.h"
#include <QWidget>

//右侧显示下一个元素的自绘widget

class NextArea : public QWidget
{
	Q_OBJECT
public:
	explicit NextArea(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);//绘制事件

public slots:
    void slotUpdateNextItem(ItemType t, ItemDirection direction);//接收下一方块更新（槽）

private:
	Item mItem;
};

#endif // NEXTAREA_H
