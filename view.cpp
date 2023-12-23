#include "view.h"

View::View(QWidget *parent)
{
    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);

    m_scene->setBackgroundBrush(Qt::black); // TMP

    m_map = new Map(QSizeF(25, 25), 35, 35);
//    connect(m_map, SIGNAL(updated()), this, SLOT(mapUpdated()));
    m_scene->addItem(m_map);
}

Map *View::getMapPtr()
{
    return m_map;
}

//void View::mapUpdated()
//{
//    this->setScene(m_scene);
//}
