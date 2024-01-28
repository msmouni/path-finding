#include "find.h"
#include <QQueue>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>

PathFinding::PathFinding(QObject *parent, Map *map, int visual_delay_ms)
    : QObject{parent}, m_map(map), m_visual_delay_ms(visual_delay_ms)
{
    m_timer.start();
}

PathFindingResult PathFinding::find()
{
    if (m_map->isReady())
    {
        reset();

        initSearch();

        while (!isQueueEmpty())
        {
            updateCurrentTile();

            TileType tile_type = m_map->getTileType(m_current_tile.getPos());

            // Note: When adding a Tile with lower cost to the priority_queue, the old Tile with greater cost remains in the priority_queue
            if (tile_type != TileType::Visited)
            {
                m_total_checks += 1;

                if (tile_type == TileType::Target)
                {
                    drawCurrentPath();

                    return PathFindingResult(true, m_total_checks, m_duration_us, m_current_tile.getPath());
                }

                if (m_platformer && m_current_tile.getPos().y() + 1 < m_map->getNbRows() && m_map->getTileType(m_current_tile.getPos().x(), m_current_tile.getPos().y() + 1) == TileType::Solid)
                {
                    m_current_tile.setCanJump();
                }

                processAdjacentTiles(m_current_tile.getPos()); // new elements are pushed to the priority_queue, so references obtained by m_priority_queue.top() will be invalid

                setCurrentTileVisited();

                visualizeCurrentPath();
            }
        }
    }

    return PathFindingResult(false, m_total_checks, m_duration_us, QVector<QPoint>());
}

void PathFinding::setVisualDelayMs(int delay_ms)
{
    m_visual_delay_ms = delay_ms;
}

void PathFinding::setPlatformer(bool set)
{
    m_platformer = set;
}

void PathFinding::drawCurrentPath()
{
    for (QPoint tile_pos : m_current_tile.getParents())
    {
        TileType tile_type = m_map->getTileType(tile_pos);
        if (tile_type != TileType::Target && tile_type != TileType::Start)
        {
            m_map->setTileType(tile_pos.x(), tile_pos.y(), TileType::Path);

            QThread::msleep(m_visual_delay_ms);

            m_map->update();
        }
    }
}

void PathFinding::visualizeCurrentPath()
{
    m_duration_us += m_timer.nsecsElapsed() / 1000;

    QHash<QPoint, TileType> actual_types;

    for (QPoint tile_pos : m_current_tile.getPath())
    {
        TileType tile_type = m_map->getTileType(tile_pos);
        if (tile_type != TileType::Start && tile_type != TileType::Current)
        {
            actual_types.insert(tile_pos, m_map->getTileType(tile_pos));
            m_map->setTileType(tile_pos, TileType::Current);
        }
    }

    m_map->update();

    QThread::msleep(m_visual_delay_ms);

    for (auto [tile_pos, tile_type] : actual_types.asKeyValueRange())
    {
        m_map->setTileType(tile_pos, tile_type);
    }

    m_timer.restart();
}

void PathFinding::tryToProcessTile(const int &tile_idx_x, const int &tile_idx_y, AdjacentTile which_tile)
{
    if (m_map->containsTile(tile_idx_x, tile_idx_y))
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);

        bool going_up = which_tile == AdjacentTile::Top || which_tile == AdjacentTile::TopLeft || which_tile == AdjacentTile::TopRight;

        bool is_tile_valid = tile_type == TileType::Empty || tile_type == TileType::Target || (m_platformer && ((tile_type == TileType::VisitedDownWay && going_up) || (tile_type == TileType::VisitedUpWay)));

        if (is_tile_valid)
        {
            switch (which_tile)
            {
            case AdjacentTile::Left:
            {
                if (!m_platformer)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                else
                {
                    bool bottom_tile_in_map = tile_idx_y + 1 < m_map->getNbRows();
                    bool current_tile_on_platform = bottom_tile_in_map && m_map->getTileType(tile_idx_x + 1, tile_idx_y + 1) == TileType::Solid;
                    bool up_way_left_tile_on_platform = bottom_tile_in_map && m_current_tile.isReachedUpWay() && m_map->getTileType(tile_idx_x, tile_idx_y + 1) == TileType::Solid;

                    if (current_tile_on_platform || up_way_left_tile_on_platform)
                    {
                        processTile(tile_idx_x, tile_idx_y);
                    }
                }

                break;
            }
            case AdjacentTile::Right:
            {
                if (!m_platformer)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                else
                {
                    bool bottom_tile_in_map = tile_idx_y + 1 < m_map->getNbRows();
                    bool current_tile_on_platform = bottom_tile_in_map && m_map->getTileType(tile_idx_x - 1, tile_idx_y + 1) == TileType::Solid;
                    bool up_way_right_tile_on_platform = bottom_tile_in_map && m_current_tile.isReachedUpWay() && m_map->getTileType(tile_idx_x, tile_idx_y + 1) == TileType::Solid;

                    if (current_tile_on_platform || up_way_right_tile_on_platform)
                    {
                        processTile(tile_idx_x, tile_idx_y);
                    }
                }
                break;
            }
            case AdjacentTile::Top:
            {
                if (!m_platformer)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                else if (m_current_tile.canJump())
                {
                    processJumpTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::TopLeft:
            {
                // Is accessible
                bool is_not_corner = m_map->getTileType(tile_idx_x, tile_idx_y + 1) != TileType::Solid && m_map->getTileType(tile_idx_x + 1, tile_idx_y) != TileType::Solid;

                if (is_not_corner)
                {
                    if (!m_platformer)
                    {
                        processTile(tile_idx_x, tile_idx_y);
                    }
                    else if (m_current_tile.canJump())
                    {
                        processJumpTile(tile_idx_x, tile_idx_y);
                    }
                }
                break;
            }
            case AdjacentTile::TopRight:
            {
                // Is accessible
                bool is_not_corner = m_map->getTileType(tile_idx_x, tile_idx_y + 1) != TileType::Solid && m_map->getTileType(tile_idx_x - 1, tile_idx_y) != TileType::Solid;

                if (is_not_corner)
                {
                    if (!m_platformer)
                    {
                        processTile(tile_idx_x, tile_idx_y);
                    }
                    else if (m_current_tile.canJump())
                    {
                        processJumpTile(tile_idx_x, tile_idx_y);
                    }
                }
                break;
            }
            case AdjacentTile::Bottom:
            {
                processTile(tile_idx_x, tile_idx_y);
                break;
            }
            case AdjacentTile::BottomLeft:
            {
                // Is accessible
                bool is_not_corner = m_map->getTileType(tile_idx_x, tile_idx_y - 1) != TileType::Solid && m_map->getTileType(tile_idx_x + 1, tile_idx_y) != TileType::Solid;

                if (is_not_corner)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::BottomRight:
            {
                // Is accessible
                bool is_not_corner = m_map->getTileType(tile_idx_x, tile_idx_y - 1) != TileType::Solid && m_map->getTileType(tile_idx_x - 1, tile_idx_y) != TileType::Solid;

                if (is_not_corner)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

void PathFinding::setCurrentTileVisited()
{
    const QPoint &current_pos = m_current_tile.getPos();
    const TileType &current_type = m_map->getTileType(current_pos);

    if (current_type != TileType::Start)
    {
        if (!m_platformer || (current_type == TileType::VisitedDownWay && m_current_tile.isReachedUpWay()) || (current_type == TileType::VisitedUpWay && !m_current_tile.isReachedUpWay()))
        {
            m_map->setTileType(current_pos, TileType::Visited);
        }
        else if (m_current_tile.isReachedUpWay())
        {
            m_map->setTileType(current_pos, TileType::VisitedUpWay);
        }
        else
        {
            m_map->setTileType(current_pos, TileType::VisitedDownWay);
        }
    }
}

void PathFinding::processAdjacentTiles(const QPoint &tile_idx)
{
    int tile_x = tile_idx.x();
    int tile_y = tile_idx.y();

    tryToProcessTile(tile_x - 1, tile_y, AdjacentTile::Left);
    tryToProcessTile(tile_x + 1, tile_y, AdjacentTile::Right);
    tryToProcessTile(tile_x, tile_y - 1, AdjacentTile::Top);
    tryToProcessTile(tile_x, tile_y + 1, AdjacentTile::Bottom);
    // Note: since we are using a queue the order of insertion matters (First being inserted, first to be checked), so we prioritize horizantal and vertical mvmt over diagonal ones
    tryToProcessTile(tile_x - 1, tile_y - 1, AdjacentTile::TopLeft);
    tryToProcessTile(tile_x - 1, tile_y + 1, AdjacentTile::BottomLeft);
    tryToProcessTile(tile_x + 1, tile_y - 1, AdjacentTile::TopRight);
    tryToProcessTile(tile_x + 1, tile_y + 1, AdjacentTile::BottomRight);
}
