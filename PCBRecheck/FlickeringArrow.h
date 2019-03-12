#pragma once

#include <QGraphicsItem>
#include <QTimer>
#include <QObject>
#include <QPainter>
#include <QDebug>

//PCB大图上闪烁的箭头
class FlickeringArrow :
	public QObject, public QGraphicsItem
{
	Q_OBJECT

private:
	QColor brushColor;
	QTimer *timer;
	bool Flash = false;
	QSize *fullImageSize;

	//定义箭头的大小与形状
	qreal sizeFactor; //箭头尺寸因子，控制箭头大小
	qreal basicArrowLength; //箭头的基础长度
	qreal basicArrowWidth; //箭头的基础宽度
	QPointF *arrowShapeArray; //描述箭头形状的点

public:
	FlickeringArrow();
	~FlickeringArrow();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setColor(const QColor &color) { brushColor = color; }
	void setFullImageSizePtr(QSize *ptr);
};

