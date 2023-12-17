#include "tile.h"

Tile::Tile(QRectF rect, TileType type) : m_rect(rect), m_type(type)
{
}

Tile::Tile(QRectF rect) : Tile(rect, TileType::Empty)
{
}

QRectF Tile::boundingRect() const
{
    return m_rect;
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::gray);
    pen.setWidth(2);
    painter->setPen(pen);
    switch (m_type)
    {
    case TileType::Solid:
    {
        painter->setBrush(Qt::black);
        break;
    }

    case TileType::Empty:
    {
        painter->setBrush(Qt::lightGray);
        break;
    }
    case TileType::Start:
    {
        painter->setBrush(Qt::green);
        break;
    }
    case TileType::Target:
    {
        painter->setBrush(Qt::red);
        break;
    }
    case TileType::Visited:
    {
        painter->setBrush(Qt::blue);
        break;
    }
    case TileType::Path:
    {
        painter->setBrush(Qt::yellow);
        break;
    }
    default:
        break;
    }

    painter->drawRect(m_rect);
}

void Tile::setType(TileType type)
{
    m_type = type;
}

TileType Tile::getType()
{
    return m_type;
}
