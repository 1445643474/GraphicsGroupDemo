#pragma once
#include <QGraphicsWidget>

class GraphicsItem : public QGraphicsWidget
{
	Q_OBJECT
public:
	GraphicsItem();
	~GraphicsItem();
	QRectF boundingRect() const override;
signals:
	void posChanged();
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
private:
	bool m_bMoving;
	QPointF m_oldPos;
};

class CustomGraphicsItem : public GraphicsItem
{
	Q_OBJECT
public:
	CustomGraphicsItem();
	~CustomGraphicsItem();
	bool itemPosChanged(QGraphicsItem*);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
private:
	bool isDragArea(QPointF pos);
	//enum {
	//	nodir,
	//	top = 0x01,
	//	bottom = 0x02,
	//	left = 0x04,
	//	right = 0x08,
	//	topLeft = 0x01 | 0x04,
	//	topRight = 0x01 | 0x08,
	//	bottomLeft = 0x02 | 0x04,
	//	bottomRight = 0x02 | 0x08
	//} resizeDir; //更改尺寸的方向
private:
	bool m_bDragging;
	QGraphicsTextItem* m_titleTextItem;
	QGraphicsTextItem* m_contentTextItem;

};