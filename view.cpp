#include "view.h"

View::View(QWidget *parent)
{
    m_scene = new QGraphicsScene(this);
    this->setScene(m_scene);

    m_scene->setBackgroundBrush(Qt::blue); // TMP

    m_map = new Map(QSizeF(25, 25), 35, 35);
    m_scene->addItem(m_map);
}
