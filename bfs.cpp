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
        QVector<BfsVisit> tile_line;
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            tile_line.append(BfsVisit());
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
            m_queued_map[i][j].setJumpCount(MAX_JUMP_COUNT);
            m_queued_map[i][j].setVisited(false);
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
    // BfsVisit is used in order to not queue a Tile twice (queued but not yet visited (m_map..))
    if (!m_queued_map[tile_idx_x][tile_idx_y].isVisited())
    {
        m_queue.enqueue(PathFindingTile(QPoint(tile_idx_x, tile_idx_y), m_current_tile));
        m_queued_map[tile_idx_x][tile_idx_y].setVisited(true);
    }
}

void Bfs::processJumpTile(const int &tile_idx_x, const int &tile_idx_y)
{
    if (m_queued_map[tile_idx_x][tile_idx_y].getJumpCount() > m_current_tile.getJumpCount())
    {
        m_queue.enqueue(PathFindingTile(QPoint(tile_idx_x, tile_idx_y), m_current_tile));
        m_queued_map[tile_idx_x][tile_idx_y].setJumpCount(m_current_tile.getJumpCount());
    }
}
