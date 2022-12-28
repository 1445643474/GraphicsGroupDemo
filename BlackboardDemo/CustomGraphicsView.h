#pragma once
#include <QGraphicsView>
class CustomGraphicsView :
    public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView();
private slots:
    void onItemPosChanged();
private:
    QGraphicsScene *m_sence;
};

