#include "map.h"

Map::Map(QSizeF tile_size, int nb_rows, int nb_columns) : m_tile_size(tile_size), m_nb_rows(nb_rows), m_nb_columns(nb_columns), m_mouse_boutton(Qt::MouseButton::NoButton), m_mouse_button_clicked(false)
{

    for (int i = 0; i < m_nb_columns; i++)
    {
        QVector<Tile *> tile_column;

        for (int j = 0; j < m_nb_rows; j++)
        {
            tile_column.append(new Tile(QRectF(i * m_tile_size.width(), j * m_tile_size.height(), m_tile_size.width(), m_tile_size.height())));

            this->addToGroup(tile_column.last());
        }
        m_tiles.append(tile_column);
    }
}

QRectF Map::boundingRect() const
{
    return childrenBoundingRect();
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse_boutton = event->button();
    m_mouse_button_clicked = true;

    m_last_mouse_indexes = getIndexesFromPointF(event->pos());
}

void Map::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse_boutton = Qt::MouseButton::NoButton;
    m_mouse_button_clicked = false;
}

void Map::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QPointF cursor_pos = event->pos();

    if (this->contains(cursor_pos))
    {
        QPoint indexes = getIndexesFromPointF(cursor_pos);

        if (!m_mouse_button_clicked && m_mouse_boutton != Qt::MouseButton::NoButton)
        {
            m_mouse_button_clicked = true;
            m_last_mouse_indexes = indexes;
        }

        if (indexes.x() == m_last_mouse_indexes.x())
        {
            for (int j = fmin(indexes.y(), m_last_mouse_indexes.y()); j <= fmax(indexes.y(), m_last_mouse_indexes.y()); j++)
            {
                tileClicked(QPoint(indexes.x(), j));
            }
        }
        else
        {
            qreal a = (indexes.y() - m_last_mouse_indexes.y()) / (indexes.x() - m_last_mouse_indexes.x());
            qreal b = indexes.y() - a * indexes.x();

            for (int i = fmin(indexes.x(), m_last_mouse_indexes.x()); i <= fmax(indexes.x(), m_last_mouse_indexes.x()); i++)
            {
                tileClicked(QPoint(i, a * i + b));
            }
        }

        m_last_mouse_indexes = indexes;

        this->update();
    }
    else
    {
        m_mouse_button_clicked = false;
    }
}

QPoint Map::getIndexesFromPointF(QPointF point)
{
    return QPoint(point.x() / m_tile_size.width(), point.y() / m_tile_size.height());
}

void Map::tileClicked(QPoint tile_indexes)
{
    switch (m_mouse_boutton)
    {
    case Qt::MouseButton::RightButton:
        m_tiles[tile_indexes.x()][tile_indexes.y()]->setType(TileType::Empty);
        break;
    case Qt::MouseButton::LeftButton:
        m_tiles[tile_indexes.x()][tile_indexes.y()]->setType(TileType::Solid);
        break;
    default:
        break;
    }
}
