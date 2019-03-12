#pragma once

#include <QGraphicsItem>
#include <QTimer>
#include <QObject>
#include <QPainter>
#include <QDebug>

//PCB��ͼ����˸�ļ�ͷ
class FlickeringArrow :
	public QObject, public QGraphicsItem
{
	Q_OBJECT

private:
	QColor brushColor;
	QTimer *timer;
	bool Flash = false;
	QSize *fullImageSize;

	//�����ͷ�Ĵ�С����״
	qreal sizeFactor; //��ͷ�ߴ����ӣ����Ƽ�ͷ��С
	qreal basicArrowLength; //��ͷ�Ļ�������
	qreal basicArrowWidth; //��ͷ�Ļ������
	QPointF *arrowShapeArray; //������ͷ��״�ĵ�

public:
	FlickeringArrow();
	~FlickeringArrow();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setColor(const QColor &color) { brushColor = color; }
	void setFullImageSizePtr(QSize *ptr);
};

