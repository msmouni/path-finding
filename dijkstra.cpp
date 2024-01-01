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
        QVector<DijkstraTile> tile_line;
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            tile_line.append(DijkstraTile(MAX_WEIGHT_VALUE, QPoint(i, j)));
        }

        m_weight_map.append(tile_line);
    }

    reset();
}

PathFindingResult Dijkstra::find()
{
    qint64 duration = 0;
    int total_checks = 0;

    if (m_map->isReady())
    {
        reset();

        QPoint start_idx = m_map->getStartIdx();
        m_weight_map[start_idx.x()][start_idx.y()].setWeight(0);

        m_priority_queue.push(m_weight_map[start_idx.x()][start_idx.y()]);

        while (!m_priority_queue.empty())
        {
            total_checks += 1;

            m_current_tile = m_priority_queue.top();

            m_priority_queue.pop();

            TileType tile_type = m_map->getTileType(m_current_tile.getIdx());

            if (tile_type == TileType::Target)
            {
                //            qDebug()<<current_parents;
                qDebug() << "Weight<" << m_current_tile.getWeight();
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
                path.append(m_current_tile.getIdx());

                return PathFindingResult(true, total_checks, duration, path);
            }
            else
            {
                // Not part of the algorithm, just for visualization
                duration += m_timer.nsecsElapsed() / 1000;
                m_map->setTileType(m_current_tile.getIdx(), TileType::Current);
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

            processAdjacentTiles(m_current_tile.getIdx()); // new elements are pushed to the priority_queue, so references obtained by m_priority_queue.top() will be invalid

            duration += m_timer.nsecsElapsed() / 1000;
            m_map->update();

            QThread::msleep(m_visual_delay_ms);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(m_current_tile.getIdx(), tile_type);
                // TMP
                for (QPoint parent : m_current_tile.getParents())
                {
                    if (m_map->getTileType(parent) != TileType::Start)
                    {
                        m_map->setTileType(parent, TileType::Visited);
                    }
                }
            }

            m_timer.restart();
        }
    }

    return PathFindingResult(false, total_checks, duration, QVector<QPoint>());
}

void Dijkstra::reinitWeightMap()
{
    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            m_weight_map[i][j].reset(MAX_WEIGHT_VALUE);
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

    m_timer.restart();
}

void Dijkstra::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    // addDijkstraTile
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {
            const QPoint &current_tile_idx = m_current_tile.getIdx();
            int current_x = current_tile_idx.x();
            int current_y = current_tile_idx.y();

            qreal weight = sqrt(pow(current_x - tile_idx_x, 2) + pow(current_y - tile_idx_y, 2)) + m_current_tile.getWeight();

            //            qDebug()<<weight;

            if (m_weight_map[tile_idx_x][tile_idx_y].getWeight() > weight)
            {
                m_weight_map[tile_idx_x][tile_idx_y].setWeight(weight);
            }

            QVector<QPoint> parents = m_current_tile.getParents();
            parents.append(QPoint(current_x, current_y));

            m_weight_map[tile_idx_x][tile_idx_y].setParent(parents);

            /*
             NOTE: When a new element is pushed into the priority queue,
                   it may lead to reallocation and invalidation of references or pointers to elements in the container,
                   including the references obtained from the previous calls of top().
             */
            m_priority_queue.push(m_weight_map[tile_idx_x][tile_idx_y]);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
            }
        }
    }
}
