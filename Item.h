#pragma once
#include <QList>
#include <QPoint>
#include <QMap>
#include <QPainter>
#include <QString>
//方块模块设计

//元素有6种类型
enum ItemType {
	ItemType_Line = 0,		//直线形
	ItemType_T,				//T形
	ItemType_L1,			//L形1
	ItemType_L2,			//L形2
	ItemType_Square,		//正方形、田字形
	ItemType_Z,				//Z形
	ItemType_MAX,
};

//每种类型的元素都有上下左右4种朝向，一般每种朝向是一个不同形状
//长条元素一共只有2个形状
//田字元素一共只有1个形状
enum ItemDirection {
	ItemDirection_Up = 0,
	ItemDirection_Right,
	ItemDirection_Down,
	ItemDirection_Left,
	ItemDirection_MAX,	//方向总数4
};

typedef	QList<QPoint> PointList;

class Item
{
public:
	Item() {}//构造函数、析构函数声明
	Item(ItemType type, ItemDirection direction);
	~Item();

    ItemType Type();                            //对外开放属性的get接口
	ItemDirection Direction();
	PointList Points();

    void New(int nRandomFactor);				//随机生成新元素，nRandomFactor是随机因子
    void InitPoints(ItemType type, ItemDirection direction);//根据元素类型、方向初始化4个坐标点，在旋转方块和创建新方块时用

    void ClearPoints();                         //清空所有方块，在新游戏时调用
    void AddPoints(const PointList& points);    //当移动方块固定时，将其4个坐标添加到已落下固定的方块中
    bool Contains(QPoint& point);               //移动方块每移动一次就它是否被包含在已落下固定的方块中
    bool Contains(int x, int y);                //判断是否到底和消除整行时用
    void MoveDown(int nRow, int y);				//第nRow行以上的部分下移y个单位，用在消除之后
    void DeleteRow(int y);                      //在已落下固定的方块中删除第y行的所有小方块的点坐标
    void Draw(QPainter& painter, int nStartX, int nStartY, int nWidth, int nHeight);//绘制(正在移动的 或已落下固定的)所有方块，nStartX, nStartY, nWidth, nHeight，设为0,0,40,40

	void ChangeDirection(int nDirection);		//传1就把方向状态+1，代表旋转90度
	void Move(int x, int y);					//横向移动x格,竖向移动y格
	void MoveTo(int x, int y);					//移动到位置(x,y)格




private:
    QString GetPointPostionText(ItemType type, ItemDirection direction);
	//将坐标位置的描述文本，转换为坐标点
    PointList TextToPointList(QString strFormat);

private:
    ItemType mType;					//本对象（如：正在移动的方块的）类型，(6种)，枚举型0-5
    ItemDirection mDirection;		//本对象（如：正在移动的方块的）方向，(4种)，枚举型0-4
    QPoint mPosition;				//本对象（如：正在移动的方块的）位置坐标
    PointList mPoints;				//本对象（如：已落下固定的方块的）所有点，QList<QPiont>型
};
