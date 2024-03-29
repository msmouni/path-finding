#include "map.h"
#include <QQueue>
#include <QCoreApplication>
#include <QElapsedTimer>

Map::Map(QSizeF tile_size, int nb_rows, int nb_columns) : m_tile_size(tile_size), m_nb_rows(nb_rows), m_nb_columns(nb_columns)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    init();
}

QRectF Map::boundingRect() const
{
    return childrenBoundingRect();
}

const QPoint &Map::getStartIdx()
{
    QMutexLocker ml(&m_mutex);

    return m_start_idx;
}

const TileType &Map::getTileType(const QPoint &indx)
{
    return getTileType(indx.x(), indx.y());
}

const TileType &Map::getTileType(const int &idx_x, const int &idx_y)
{
    QMutexLocker ml(&m_mutex);

    return m_tiles[idx_x][idx_y]->getType();
}

void Map::setTileType(const QPoint &idx, TileType tile_type)
{
    setTileType(idx.x(), idx.y(), tile_type);
}

void Map::setTileType(const int &idx_x, const int &idx_y, TileType tile_type)
{
    QMutexLocker ml(&m_mutex);

    m_tiles[idx_x][idx_y]->setType(tile_type);
}

const int &Map::getNbRows()
{
    QMutexLocker ml(&m_mutex);

    return m_nb_rows;
}

const int &Map::getNbColumns()
{
    QMutexLocker ml(&m_mutex);

    return m_nb_columns;
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint cursor_idx = getIndexesFromPointF(event->pos());

    if (m_s_key_pressed && event->button() == Qt::MouseButton::LeftButton)
    {

        if (m_start_set)
        {

            setTileType(getStartIdx(), TileType::Empty);
        }

        if (getTileType(cursor_idx) != TileType::Target)
        {

            m_start_set = true;
            setStartIdx(cursor_idx);
            setTileType(getStartIdx(), TileType::Start);
        }

        this->update();
    }
    else if (m_t_key_pressed && event->button() == Qt::MouseButton::LeftButton)
    {
        if (m_target_set)
        {
            setTileType(getTargetIdx(), TileType::Empty);
        }

        if (getTileType(cursor_idx) != TileType::Start)
        {
            m_target_set = true;
            setTargetIdx(cursor_idx);
            setTileType(getTargetIdx(), TileType::Target);

            this->update();
        }
    }
    else
    {
        m_mouse_boutton = event->button();
        m_mouse_button_clicked = true;
        m_last_mouse_indexes = cursor_idx;
        tileClicked(cursor_idx);

        this->update();
    }

    clearVisited();
    emit stopPathFinding();
}

void Map::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit updated();
    tryToFindPath();
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

        if (m_mouse_button_clicked)
        {

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
    }
    else
    {
        m_mouse_button_clicked = false;
    }
}

void Map::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_S:
        m_s_key_pressed = true;
        break;
    case Qt::Key_T:
        m_t_key_pressed = true;
        break;
    default:
        break;
    }
}

void Map::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_S:
        m_s_key_pressed = false;
        break;
    case Qt::Key_T:
        m_t_key_pressed = false;
        break;
    default:
        break;
    }
}

void Map::focusOutEvent(QFocusEvent *event)
{
    m_mouse_boutton = Qt::MouseButton::NoButton;
    m_mouse_button_clicked = false;
    m_s_key_pressed = false;
    m_t_key_pressed = false;

    QGraphicsItem::focusOutEvent(event);
}

void Map::init(QVector<QVector<TileType>> tiles_type)
{
    emit stopPathFinding();

    QMutexLocker ml(&m_mutex);

    bool get_tiles_type = false;

    if (!tiles_type.isEmpty())
    {
        get_tiles_type = true;
        m_nb_columns = tiles_type.length();
        m_nb_rows = tiles_type.first().length();
    }

    m_mouse_boutton = Qt::MouseButton::NoButton;
    m_mouse_button_clicked = false;
    m_s_key_pressed = false;
    m_t_key_pressed = false;
    m_start_set = false;
    m_target_set = false;

    for (QVector<Tile *> &item_vec : m_tiles)
    {
        for (Tile *item : item_vec)
        {
            /*
             * NOTE: removeFromGroup() Removes the specified item from this group. The item will be reparented to this group's parent item, or to 0 if this group has no parent.
             * Its position and transformation relative to the scene will stay intact.
             */
            this->removeFromGroup(item);

            delete item;
        }
    }

    m_tiles.clear();

    for (int i = 0; i < m_nb_columns; i++)
    {
        QVector<Tile *> tile_column;

        for (int j = 0; j < m_nb_rows; j++)
        {
            TileType tile_type = TileType::Empty;
            if (get_tiles_type)
            {
                tile_type = tiles_type[i][j];
            }

            if (tile_type == TileType::Start)
            {
                m_start_idx = QPoint(i, j);
                m_start_set = true;
            }
            else if (tile_type == TileType::Target)
            {
                m_target_idx = QPoint(i, j);
                m_target_set = true;
            }
            tile_column.append(new Tile(QRectF(i * m_tile_size.width(), j * m_tile_size.height(), m_tile_size.width(), m_tile_size.height()), tile_type));

            this->addToGroup(tile_column.last());
        }
        m_tiles.append(tile_column);
    }

    ml.unlock();

    emit updated();
}

void Map::setStartIdx(QPoint idx)
{
    QMutexLocker ml(&m_mutex);

    m_start_idx = idx;
}

void Map::setTargetIdx(QPoint idx)
{
    QMutexLocker ml(&m_mutex);

    m_target_idx = idx;
}

const QPoint &Map::getTargetIdx()
{
    QMutexLocker ml(&m_mutex);

    return m_target_idx;
}

QPoint Map::getIndexesFromPointF(QPointF point)
{
    return QPoint(point.x() / m_tile_size.width(), point.y() / m_tile_size.height());
}

void Map::tileClicked(QPoint tile_indexes)
{
    TileType tile_type = getTileType(tile_indexes);

    switch (m_mouse_boutton)
    {
    case Qt::MouseButton::RightButton:
        if (tile_type == TileType::Start)
        {
            m_start_set = false;
            clearVisited();
        }
        else if (tile_type == TileType::Target)
        {
            m_target_set = false;
            clearVisited();
        }
        setTileType(tile_indexes, TileType::Empty);
        break;
    case Qt::MouseButton::LeftButton:
    {
        if (tile_type != TileType::Start && tile_type != TileType::Target)
        {
            setTileType(tile_indexes, TileType::Solid);
        }
        break;
    }
    default:
        break;
    }
}

void Map::tryToFindPath()
{
    if (isReady())
    {
        emit findPath();
    }
}

void Map::clearVisited()
{
    QMutexLocker ml(&m_mutex);

    for (const QVector<Tile *> &tile_column : m_tiles)
    {
        for (Tile *tile : tile_column)
        {
            TileType tile_type = tile->getType();
            if (tile->isVisited() || tile_type == TileType::Path || tile_type == TileType::Current)
            {
                tile->setType(TileType::Empty);
            }
        }
    }
}

bool Map::isReady()
{
    return isTargetSet() && isStartSet();
}

bool Map::isTargetSet()
{
    return m_target_set;
}

bool Map::isStartSet()
{
    return m_start_set;
}

bool Map::containsTile(const int &pos_x, const int &pos_y)
{
    return 0 <= pos_x && pos_x < getNbColumns() && 0 <= pos_y && pos_y < getNbRows();
}

bool Map::containsTile(const QPoint &pos)
{
    return containsTile(pos.x(), pos.y());
}

void Map::loadMap(QVector<QVector<TileType>> tiles_type)
{
    init(tiles_type);
    clearVisited();
}

void Map::setNbRows(int nb_rows)
{
    QMutexLocker ml(&m_mutex);
    m_nb_rows = nb_rows;
    ml.unlock();

    init();

    this->update();
}

void Map::setNbColumns(int nb_columns)
{
    QMutexLocker ml(&m_mutex);
    m_nb_columns = nb_columns;
    ml.unlock();

    init();

    this->update();
}
