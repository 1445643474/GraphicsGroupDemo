#include "CustomGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QCursor>
#include <QPainter>
#include <QDebug>

#define min(a,b) ((a)<(b)? (a) :(b))
#define max(a, b) ((a) > (b) ? (a) : (b))

GraphicsItem::GraphicsItem() : QGraphicsWidget(), m_bMoving(false)
{
	resize(QSizeF(100, 50));
	setAutoFillBackground(true);
	setPalette(QPalette(QColor(173, 241, 174)));
	setFlag(QGraphicsItem::ItemIsMovable);

}

GraphicsItem::~GraphicsItem()
{

}

QVariant GraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemPositionHasChanged)
	{
		m_bMoving = true;
	}
	return value;
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsWidget::mouseReleaseEvent(event);
	if (m_bMoving)
	{
		m_bMoving = false;
		QPointF newPos = event->scenePos();
		if (newPos != m_oldPos)
		{
			emit posChanged();
		}
	}
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_oldPos = event->scenePos();
}

QRectF GraphicsItem::boundingRect() const
{
	return QGraphicsWidget::boundingRect();
}

CustomGraphicsItem::CustomGraphicsItem() : GraphicsItem(), m_bDragging(false)
{
	setOpacity(0.5);
	setAutoFillBackground(false);
	resize(QSizeF(300, 300));
	setAcceptHoverEvents(true);
	setMinimumSize(200, 100);
	m_titleTextItem = new QGraphicsTextItem("title", this);
	m_titleTextItem->setTextInteractionFlags(Qt::TextEditorInteraction);
	m_contentTextItem = new QGraphicsTextItem("content", this);
	m_contentTextItem->setTextInteractionFlags(Qt::TextEditorInteraction);
	m_titleTextItem->setParentItem(this);
	m_contentTextItem->setParentItem(this);
	m_titleTextItem->setPos(mapFromItem(this, 0, 0));
	m_contentTextItem->setPos(mapFromItem(this, 0, 30));
}

CustomGraphicsItem::~CustomGraphicsItem()
{
}

bool CustomGraphicsItem::itemPosChanged(QGraphicsItem* item)
{
	if (this->sceneBoundingRect().contains(item->sceneBoundingRect())
		&& item->parentItem() != this)
	{
		if (item->parentItem()
			&& item->parentItem()->sceneBoundingRect().contains(item->sceneBoundingRect())
			&& (!item->parentItem()->sceneBoundingRect().contains(this->sceneBoundingRect())))
		{
			return false;
		}
		item->setPos(mapFromItem(item, 0, 0));
		item->setParentItem(this);
		update();
		return true;
	}
	else if ((!this->sceneBoundingRect().contains(item->sceneBoundingRect())) && item->parentItem() == this)
	{
		QGraphicsItem* newParent = this->parentItem();
		while (dynamic_cast<CustomGraphicsItem*>(newParent)) {
			CustomGraphicsItem* pItem = dynamic_cast<CustomGraphicsItem*>(newParent);
			bool isUpdate = pItem->itemPosChanged(item);
			if (!isUpdate)
				newParent = pItem->parentItem();
			else
				return true;
		}
		QPointF oldPos = item->mapToItem(newParent, 0, 0);
		item->setParentItem(newParent);
		item->setPos(oldPos);
	}
	return false;
}

void CustomGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	GraphicsItem::mousePressEvent(event);

	QPointF pos = event->pos();
	if (isDragArea(pos)) {
		m_bDragging = true;
	}
	else {
		m_bDragging = false;
	}
}

void CustomGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_bDragging)
	{
		QPointF topLeft = sceneBoundingRect().topLeft();
		QPointF newPos = event->scenePos();
		QPointF currPos = sceneBoundingRect().bottomRight();

		qreal newWidth = newPos.x() - topLeft.x();
		qreal newHeight = newPos.y() - topLeft.y();
		//int ptop, pbottom, pleft, pright;                   //窗口上下左右的值
		//QRectF rect = this->geometry();
		//ptop = rect.top();
		//pbottom = rect.bottom();
		//pleft = rect.left();
		//pright = rect.right();
		//qDebug() << "------" << pright;
		//if (resizeDir & top) {                               //检测更改尺寸方向中包含的上下左右分量
		//	if (rect.height() == minimumHeight()) {
		//		ptop = min(event->scenePos().y(), ptop);
		//	}
		//	else if (rect.height() == maximumHeight()) {
		//		ptop = max(event->scenePos().y(), ptop);
		//	}
		//	else {
		//		ptop = event->scenePos().y();
		//	}
		//	//setPos(scenePos().x(), scenePos().y() + ptop);
		//}
		//else if (resizeDir & bottom) {
		//	if (rect.height() == minimumHeight()) {
		//		pbottom = max(event->scenePos().y(), ptop);
		//	}
		//	else if (rect.height() == maximumHeight()) {
		//		pbottom = min(event->scenePos().y(), ptop);
		//	}
		//	else {
		//		pbottom = event->scenePos().y();
		//	}
		//	
		//}

		//if (resizeDir & left) {                        //检测左右分量
		//	if (rect.width() == minimumWidth()) {
		//		pleft = min(event->scenePos().x(), pleft);
		//	}
		//	else if (rect.width() == maximumWidth()) {
		//		pleft = max(event->scenePos().x(), pleft);
		//	}
		//	else {
		//		pleft = event->scenePos().x();
		//	}
		//	//setPos(scenePos().x() + pleft, scenePos().y());
		//}
		//else if (resizeDir & right) {
		//	if (rect.width() == minimumWidth()) {
		//		pright = max(event->scenePos().x(), pright);
		//	}
		//	else if (rect.width() == maximumWidth()) {
		//		pright = min(event->scenePos().x(), pright);
		//	}
		//	else {
		//		pright = event->scenePos().x();
		//	}
		//}

		//setGeometry(mapRectToScene(QRectF(QPoint(pleft, ptop), QPoint(pright, pbottom))));
		resize(newWidth, newHeight);
		emit posChanged();
		return;
	}

	QGraphicsWidget::mouseMoveEvent(event);
}

void CustomGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_bDragging) {
		m_bDragging = false;
		return;
	}
	GraphicsItem::mouseReleaseEvent(event);
}

void CustomGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	QCursor cursor;
	bool bDrag = isDragArea(event->pos());
	if (bDrag) {
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else {
		setCursor(QCursor(Qt::ArrowCursor));
	}
	GraphicsItem::hoverMoveEvent(event);
}

bool CustomGraphicsItem::isDragArea(QPointF pos)
{
	//QRectF rect = boundingRect();
	//int diffLeft = pos.x() - rect.left();      //计算鼠标距离窗口上下左右有多少距离
	//int diffRight = pos.x() - rect.right();
	//int diffTop = pos.y() - rect.top() - 30;
	//int diffBottom = pos.y() - rect.bottom();

	//Qt::CursorShape cursorShape;

	//if (diffTop < 10 && diffTop >= 0) {                              //根据 边缘距离 分类改变尺寸的方向
	//	if (diffLeft < 10 && diffLeft >= 0) {
	//		resizeDir = topLeft;
	//		cursorShape = Qt::SizeFDiagCursor;

	//	}
	//	else if (diffRight > -10 && diffRight <= 0) {
	//		resizeDir = topRight;
	//		cursorShape = Qt::SizeBDiagCursor;

	//	}
	//	else {
	//		resizeDir = top;
	//		cursorShape = Qt::SizeVerCursor;

	//	}

	//}
	//else if (abs(diffBottom) < 10 && diffBottom <= 0) {
	//	if (diffLeft < 10 && diffLeft >= 0) {
	//		resizeDir = bottomLeft;
	//		cursorShape = Qt::SizeBDiagCursor;

	//	}
	//	else if (diffRight > -10 && diffRight <= 0) {
	//		resizeDir = bottomRight;
	//		cursorShape = Qt::SizeFDiagCursor;

	//	}
	//	else {
	//		resizeDir = bottom;
	//		cursorShape = Qt::SizeVerCursor;

	//	}

	//}
	//else if (abs(diffLeft) < 10) {
	//	resizeDir = left;
	//	cursorShape = Qt::SizeHorCursor;

	//}
	//else if (abs(diffRight) < 10) {
	//	resizeDir = right;
	//	cursorShape = Qt::SizeHorCursor;

	//}
	//else {
	//	resizeDir = nodir;
	//	cursorShape = Qt::ArrowCursor;

	//}
	//cursor.setShape(cursorShape);
	QPointF bottomright = boundingRect().bottomRight();
	QPointF offset = pos - bottomright;
	return (offset.manhattanLength() < 20);
}

void CustomGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->fillRect(QRectF(0, 30, this->boundingRect().width(), this->boundingRect().height() - 30), QColor("#234788"));
	GraphicsItem::paint(painter, option, widget);
}