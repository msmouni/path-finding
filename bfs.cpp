#include "bfs.h"
#include <QThread>

Bfs::Bfs(QObject *parent, Map *map)
    : PathFinding{parent, map}
{
    init();
}

void Bfs::init()
{
    m_queued_map.clear();

    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        QVector<bool> tile_line;
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            tile_line.append(false);
        }

        m_queued_map.append(tile_line);
    }

    reset();
}

void Bfs::reinitQueuedMap()
{
    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            m_queued_map[i][j] = false;
        }
    }
}

void Bfs::reset()
{
    m_map->clearVisited();

    reinitQueuedMap();

    m_queue.clear();

    m_duration_us = 0;
    m_total_checks = 0;

    m_timer.restart();
}

void Bfs::initSearch()
{
    m_queue.enqueue(PathFindingTile(m_map->getStartIdx()));
}

void Bfs::updateCurrentTile()
{
    m_current_tile = m_queue.dequeue();
}

bool Bfs::isQueueEmpty()
{
    return m_queue.isEmpty();
}

void Bfs::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    // addTileToBfsQueue
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        if (!m_queued_map[tile_idx_x][tile_idx_y])
        {
            TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);
            if (tile_type == TileType::Empty || tile_type == TileType::Target)
            {
                m_queue.enqueue(PathFindingTile(QPoint(tile_idx_x, tile_idx_y), m_current_tile));
                m_queued_map[tile_idx_x][tile_idx_y] = true;
            }
        }
    }
}
