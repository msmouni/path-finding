#include "a_star.h"
#include <QThread>

Astar::Astar(QObject *parent, Map *map)
    : PathFinding{parent, map}
{
    init();
}

void Astar::init()
{
    m_weight_map.clear();

    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        QVector<AstarCost> tile_line;
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            tile_line.append(AstarCost(MAX_WEIGHT_VALUE, 0));
        }

        m_weight_map.append(tile_line);
    }

    reset();
}

void Astar::reinitWeightMap()
{
    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            m_weight_map[i][j].setCost(MAX_WEIGHT_VALUE);
            m_weight_map[i][j].setTargetCost(0);
        }
    }
}

qreal Astar::getEstimatedTargetCost(const QPoint &idx)
{
    return getEstimatedTargetCost(idx.x(), idx.y());
}

qreal Astar::getEstimatedTargetCost(const int &idx_x, const int &idx_y)
{
    QPoint target_idx = m_map->getTargetIdx();

    // TO change later (maybe)
    return sqrt(pow(target_idx.rx() - idx_x, 2) + pow(target_idx.ry() - idx_y, 2));
}

void Astar::reset()
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

void Astar::initSearch()
{
    QPoint start_idx = m_map->getStartIdx();

    m_weight_map[start_idx.x()][start_idx.y()].setCost(0);

    qreal estimated_target_cost = getEstimatedTargetCost(start_idx);
    m_weight_map[start_idx.x()][start_idx.y()].setTargetCost(estimated_target_cost);

    m_priority_queue.push(AstarTile(start_idx, 0, estimated_target_cost));
}

void Astar::updateCurrentTile()
{
    m_current_tile = m_priority_queue.top().getTile();

    m_priority_queue.pop();
}

bool Astar::isQueueEmpty()
{
    return m_priority_queue.empty();
}

void Astar::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {
            const QPoint &current_tile_idx = m_current_tile.getPos();
            int current_x = current_tile_idx.x();
            int current_y = current_tile_idx.y();

            qreal cost = sqrt(pow(current_x - tile_idx_x, 2) + pow(current_y - tile_idx_y, 2)) + m_weight_map[current_x][current_y].getCost();

            if (m_weight_map[tile_idx_x][tile_idx_y].getCost() > cost)
            {
                m_weight_map[tile_idx_x][tile_idx_y].setCost(cost);

                qreal target_cost = getEstimatedTargetCost(tile_idx_x, tile_idx_y);
                m_weight_map[tile_idx_x][tile_idx_y].setTargetCost(target_cost);

                /*
                 NOTE: When a new element is pushed into the priority queue,
                       it may lead to reallocation and invalidation of references or pointers to elements in the container,
                       including the references obtained from the previous calls of top().
                 */
                m_priority_queue.push(AstarTile(QPoint(tile_idx_x, tile_idx_y), cost, target_cost, m_current_tile));
            }
        }
    }
}
