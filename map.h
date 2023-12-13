#ifndef MAP_H
#define MAP_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include "tile.h"

class Map : public QGraphicsItemGroup
{
public:
    Map(QSizeF tile_size, int nb_rows, int nb_columns);

    QRectF boundingRect() const override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    Qt::MouseButton m_mouse_boutton;
    bool m_mouse_button_clicked;
    QPoint m_last_mouse_indexes;

    QVector<QVector<Tile *>> m_tiles;

    QPoint getIndexesFromPointF(QPointF point);
    void tileClicked(QPoint tile_indexes);
};

#endif // MAP_H
