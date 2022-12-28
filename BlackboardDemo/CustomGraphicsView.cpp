#include "CustomGraphicsView.h"
#include "CustomGraphicsItem.h"

CustomGraphicsView::CustomGraphicsView()
{
	m_sence = new QGraphicsScene(-100, -100, 1000, 1000);
	setScene(m_sence);
	setRenderHint(QPainter::Antialiasing);
	setWindowFlag(Qt::WindowMinMaxButtonsHint, false);

	for (int i = 0; i < 2; i++)
	{
		CustomGraphicsItem* item = new CustomGraphicsItem;
		item->setPos(-50, 350*i);
		m_sence->addItem(item);
		connect(item, &GraphicsItem::posChanged, this, &CustomGraphicsView::onItemPosChanged);
	}
	for (int i = 0; i < 4; i++)
	{
		GraphicsItem* item = new GraphicsItem;
		item->setPos(300, i * 205);
		m_sence->addItem(item);
		connect(item, &GraphicsItem::posChanged, this, &CustomGraphicsView::onItemPosChanged);
	}
}

void CustomGraphicsView::onItemPosChanged()
{
	GraphicsItem* senderItem = dynamic_cast<GraphicsItem*>(sender());
	CustomGraphicsItem* groupItem = dynamic_cast<CustomGraphicsItem*>(senderItem);
	for (auto item : m_sence->items())
	{
		if (item == senderItem)
			continue;
		CustomGraphicsItem* groupItem1 = dynamic_cast<CustomGraphicsItem*>(item);
		if (groupItem)
		{
			if (groupItem->childItems().contains(item))
			{
				if (groupItem1)
				{
					emit groupItem1->posChanged();
				}
				continue;
			}
			if (groupItem1)
			{
				groupItem1->itemPosChanged(senderItem);
			}
			groupItem->itemPosChanged(item);
		}
		else if(groupItem1)
		{
				groupItem1->itemPosChanged(senderItem);
		}
	}
}