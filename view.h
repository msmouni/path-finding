#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "map.h"

class View : public QGraphicsView
{
public:
    View(QWidget *parent = nullptr);

    Map *getMapPtr();

private:
    QGraphicsScene *m_scene;

    Map *m_map;
};

#endif // VIEW_H
