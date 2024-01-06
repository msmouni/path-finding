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

void PathFinding::processAdjacentTiles(const QPoint &tile_idx)
{
    int tile_x = tile_idx.x();
    int tile_y = tile_idx.y();

    processTile(tile_x - 1, tile_y);
    processTile(tile_x + 1, tile_y);
    processTile(tile_x, tile_y - 1);
    processTile(tile_x, tile_y + 1);

    // Note: since we are using a queue the order of insertion matters (First being inserted, first to be checked), so we prioritize horizantal and vertical mvmt over diagonal ones
    int map_rows_nb = m_map->getNbRows();
    int map_columns_nb = m_map->getNbColumns();

    if (tile_x > 0 && m_map->getTileType(tile_x - 1, tile_y) != TileType::Solid && tile_y > 0 && m_map->getTileType(tile_x, tile_y - 1) != TileType::Solid)
    {
        processTile(tile_x - 1, tile_y - 1);
    }
    if (tile_x > 0 && m_map->getTileType(tile_x - 1, tile_y) != TileType::Solid && tile_y < map_rows_nb - 1 && m_map->getTileType(tile_x, tile_y + 1) != TileType::Solid)
    {
        processTile(tile_x - 1, tile_y + 1);
    }
    if (tile_x < map_columns_nb - 1 && m_map->getTileType(tile_x + 1, tile_y) != TileType::Solid && tile_y > 0 && m_map->getTileType(tile_x, tile_y - 1) != TileType::Solid)
    {
        processTile(tile_x + 1, tile_y - 1);
    }
    if (tile_x < map_columns_nb - 1 && m_map->getTileType(tile_x + 1, tile_y) != TileType::Solid && tile_y < map_rows_nb - 1 && m_map->getTileType(tile_x, tile_y + 1) != TileType::Solid)
    {
        processTile(tile_x + 1, tile_y + 1);
    }
}
