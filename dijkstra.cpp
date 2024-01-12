#include "dijkstra.h"
#include <QQueue>
#include <QThread>

Dijkstra::Dijkstra(QObject *parent, Map *map)
    : PathFinding{parent, map}
{
    init();
}

void Dijkstra::init()
{
    m_weight_map.clear();

    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        QVector<DijkstraCost> tile_line;
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            tile_line.append(DijkstraCost(MAX_WEIGHT_VALUE, MAX_JUMP_COUNT));
        }

        m_weight_map.append(tile_line);
    }

    reset();
}

void Dijkstra::reinitWeightMap()
{
    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            m_weight_map[i][j].setJumpCost(MAX_WEIGHT_VALUE, MAX_JUMP_COUNT);
        }
    }
}

void Dijkstra::reset()
{
    m_map->clearVisited();

    reinitWeightMap();

    while (!m_priority_queue.empty())
    {
        m_priority_queue.pop();
    }

    m_duration_us = 0;
    m_total_checks = 0;

    m_timer.restart();
}

void Dijkstra::initSearch()
{
    QPoint start_idx = m_map->getStartIdx();
    m_weight_map[start_idx.x()][start_idx.y()].setJumpCost(0, 0);

    m_priority_queue.push(DijkstraTile(start_idx));
}

void Dijkstra::updateCurrentTile()
{
    m_current_tile = m_priority_queue.top().getTile();
    m_current_tile_weight = m_priority_queue.top().getWeight();

    m_priority_queue.pop();
}

bool Dijkstra::isQueueEmpty()
{
    return m_priority_queue.empty();
}

void Dijkstra::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    const QPoint &current_tile_idx = m_current_tile.getPos();

    qreal weight = sqrt(pow(current_tile_idx.x() - tile_idx_x, 2) + pow(current_tile_idx.y() - tile_idx_y, 2)) + m_current_tile_weight;

    if (m_weight_map[tile_idx_x][tile_idx_y].getCost() > weight)
    {
        m_weight_map[tile_idx_x][tile_idx_y].setCost(weight);

        /*
        NOTE: When a new element is pushed into the priority queue,
            it may lead to reallocation and invalidation of references or pointers to elements in the container,
            including the references obtained from the previous calls of top().
        */
        m_priority_queue.push(DijkstraTile(QPoint(tile_idx_x, tile_idx_y), weight, m_current_tile));
    }
}

void Dijkstra::processJumpTile(const int &tile_idx_x, const int &tile_idx_y)
{
    const QPoint &current_tile_idx = m_current_tile.getPos();

    qreal weight = sqrt(pow(current_tile_idx.x() - tile_idx_x, 2) + pow(current_tile_idx.y() - tile_idx_y, 2)) + m_current_tile_weight;

    if (m_weight_map[tile_idx_x][tile_idx_y].getCost() > weight || m_weight_map[tile_idx_x][tile_idx_y].getNbJump() > m_current_tile.getJumpCount())
    {
        m_weight_map[tile_idx_x][tile_idx_y].setJumpCost(weight, m_current_tile.getJumpCount());

        /*
        NOTE: When a new element is pushed into the priority queue,
            it may lead to reallocation and invalidation of references or pointers to elements in the container,
            including the references obtained from the previous calls of top().
        */
        m_priority_queue.push(DijkstraTile(QPoint(tile_idx_x, tile_idx_y), weight, m_current_tile));
    }
}
