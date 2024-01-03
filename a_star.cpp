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

PathFindingResult Astar::find()
{
    qint64 duration = 0;
    int total_checks = 0;

    if (m_map->isReady())
    {
        reset();

        QPoint start_idx = m_map->getStartIdx();

        m_weight_map[start_idx.x()][start_idx.y()].setCost(0);

        qreal estimated_target_cost = getEstimatedTargetCost(start_idx);
        m_weight_map[start_idx.x()][start_idx.y()].setTargetCost(estimated_target_cost);

        m_priority_queue.push(AstarTile(start_idx, 0, estimated_target_cost));

        while (!m_priority_queue.empty())
        {
            m_current_tile = m_priority_queue.top().getTile();

            m_priority_queue.pop();

            TileType tile_type = m_map->getTileType(m_current_tile.getPos());

            // Note: When adding a Tile with lower cost to the priority_queue, the old Tile with greater cost remains in the priority_queue
            if (tile_type != TileType::Visited)
            {
                total_checks += 1;

                if (tile_type == TileType::Target)
                {
                    //            qDebug()<<current_parents;
                    qDebug() << "Cost" << m_weight_map[m_current_tile.getPos().x()][m_current_tile.getPos().y()].getWeight();
                    qDebug() << "elapsed time" << duration << " us";
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

                    QVector<QPoint> path = m_current_tile.getParents();
                    path.append(m_current_tile.getPos());

                    return PathFindingResult(true, total_checks, duration, path);
                }
                else
                {
                    // Not part of the algorithm, just for visualization
                    duration += m_timer.nsecsElapsed() / 1000;
                    if (tile_type != TileType::Start)
                    {
                        m_map->setTileType(m_current_tile.getPos(), TileType::Current);
                    }

                    // TMP
                    for (QPoint parent : m_current_tile.getParents())
                    {
                        if (m_map->getTileType(parent) != TileType::Start)
                        {
                            m_map->setTileType(parent, TileType::Current);
                        }
                    }

                    m_timer.restart();
                }

                processAdjacentTiles(m_current_tile.getPos()); // new elements are pushed to the priority_queue, so references obtained by m_priority_queue.top() will be invalid

                duration += m_timer.nsecsElapsed() / 1000;
                m_map->update();

                QThread::msleep(m_visual_delay_ms);

                if (tile_type != TileType::Start)
                {
                    m_map->setTileType(m_current_tile.getPos(), TileType::Visited);
                }
                // TMP
                for (QPoint parent : m_current_tile.getParents())
                {
                    if (m_map->getTileType(parent) != TileType::Start)
                    {
                        m_map->setTileType(parent, TileType::Visited);
                    }
                }

                m_timer.restart();
            }
        }
    }

    return PathFindingResult(false, total_checks, duration, QVector<QPoint>());
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

    m_timer.restart();
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
