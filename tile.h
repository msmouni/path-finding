#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QPainter>

enum class TileType
{
    Empty,
    Solid,
    Start,
    Target,
    VisitedUpWay,
    VisitedDownWay,
    Visited,
    Current,
    Path,
};

class Tile : public QGraphicsItem
{
public:
    Tile(QRectF rect, TileType type);
    Tile(QRectF rect);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setType(TileType type);
    const TileType &getType();
    bool isVisited();

private:
    QRectF m_rect;
    TileType m_type;
};

#endif // TILE_H
