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

                processAdjacentTiles(m_current_tile.getPos()); // new elements are pushed to the priority_queue, so references obtained by m_priority_queue.top() will be invalid

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

    for (QPoint tile_pos : m_current_tile.getPath())
    {
        if (m_map->getTileType(tile_pos) != TileType::Start)
        {
            m_map->setTileType(tile_pos, TileType::Current);
        }
    }

    m_map->update();

    QThread::msleep(m_visual_delay_ms);

    for (QPoint tile_pos : m_current_tile.getPath())
    {
        if (m_map->getTileType(tile_pos) != TileType::Start)
        {
            m_map->setTileType(tile_pos, TileType::Visited);
        }
    }

    m_timer.restart();
}

void PathFinding::tryToProcessTile(const int &tile_idx_x, const int &tile_idx_y, AdjacentTile which_tile)
{
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {
            switch (which_tile)
            {
            case AdjacentTile::Left:
            {
                processTile(tile_idx_x, tile_idx_y);
                break;
            }
            case AdjacentTile::Right:
            {
                processTile(tile_idx_x, tile_idx_y);
                break;
            }
            case AdjacentTile::Top:
            {
                processTile(tile_idx_x, tile_idx_y);
                break;
            }
            case AdjacentTile::Bottom:
            {
                processTile(tile_idx_x, tile_idx_y);
                break;
            }
            case AdjacentTile::TopLeft:
            {
                if (m_map->getTileType(tile_idx_x, tile_idx_y + 1) != TileType::Solid && m_map->getTileType(tile_idx_x + 1, tile_idx_y) != TileType::Solid)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::BottomLeft:
            {
                if (m_map->getTileType(tile_idx_x, tile_idx_y - 1) != TileType::Solid && m_map->getTileType(tile_idx_x + 1, tile_idx_y) != TileType::Solid)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::TopRight:
            {
                if (m_map->getTileType(tile_idx_x, tile_idx_y + 1) != TileType::Solid && m_map->getTileType(tile_idx_x - 1, tile_idx_y) != TileType::Solid)
                {
                    processTile(tile_idx_x, tile_idx_y);
                }
                break;
            }
            case AdjacentTile::BottomRight:
            {
                if (m_map->getTileType(tile_idx_x, tile_idx_y - 1) != TileType::Solid && m_map->getTileType(tile_idx_x - 1, tile_idx_y) != TileType::Solid)
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
