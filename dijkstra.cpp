#include "dijkstra.h"
#include <QQueue>
#include <QThread>

Dijkstra::Dijkstra(QObject *parent, Map *map)
    : PathFinding{parent, map}
{
    for (int i = 0; i < map->getNbColumns(); i++)
    {
        QVector<DijkstraTile> tile_line;
        for (int j = 0; j < map->getNbRows(); j++)
        {
            tile_line.append(DijkstraTile(MAX_WEIGHT_VALUE, QPoint(i, j)));
        }

        m_weight_map.append(tile_line);
    }
}

void Dijkstra::find()
{
    reinitWeightMap();

    m_timer.restart();

    qint64 duration = 0;

    // Note reg std::multiset : https://stackoverflow.com/questions/5895792/why-is-using-a-stdmultiset-as-a-priority-queue-faster-than-using-a-stdpriori
    std::priority_queue<DijkstraTile> priority_queue;

    QPoint start_idx = m_map->getStartIdx();
    m_weight_map[start_idx.x()][start_idx.y()].setWeight(0);

    priority_queue.push(m_weight_map[start_idx.x()][start_idx.y()]);

    while (!priority_queue.empty())
    {
        DijkstraTile current_tile = priority_queue.top();

        TileType tile_type = m_map->getTileType(current_tile.getIdx());

        if (tile_type == TileType::Target)
        {
            //            qDebug()<<current_parents;
            qDebug() << "elapsed time" << duration << " us";
            for (QPoint tile_pos : current_tile.getParents())
            {
                TileType tile_type = m_map->getTileType(tile_pos);
                if (tile_type != TileType::Target && tile_type != TileType::Start)
                {
                    m_map->setTileType(tile_pos.x(), tile_pos.y(), TileType::Path);

                    QThread::msleep(10);

                    m_map->update();
                }
            }

            return;
        }

        processAdjacentTiles(current_tile.getIdx(), [this, &priority_queue, &current_tile](int x, int y)
                             {
            // Lambda implementation
            addDijkstraTile(x, y, priority_queue, current_tile); });

        priority_queue.pop();

        duration += m_timer.nsecsElapsed() / 1000;
        m_map->update();

        QThread::msleep(5);

        m_timer.restart();
    }
}

void Dijkstra::reinitWeightMap()
{
    for (int i = 0; i < m_map->getNbColumns(); i++)
    {
        for (int j = 0; j < m_map->getNbRows(); j++)
        {
            m_weight_map[i][j].reinit(MAX_WEIGHT_VALUE);
        }
    }
}

void Dijkstra::addDijkstraTile(int idx_x, int idx_y, std::priority_queue<DijkstraTile> &priority_queue, DijkstraTile &current_tile)
{
    if (0 <= idx_x && idx_x < m_map->getNbColumns() && 0 <= idx_y && idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(idx_x, idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {
            int &current_x = current_tile.getIdx().rx();
            int &current_y = current_tile.getIdx().ry();

            qreal weight = sqrt(pow(current_x - idx_x, 2) + pow(current_y - idx_y, 2)) + current_tile.getWeight();

            //            qDebug()<<weight;

            if (m_weight_map[idx_x][idx_y].getWeight() > weight)
            {
                m_weight_map[idx_x][idx_y].setWeight(weight);
            }

            QVector<QPoint> parents = current_tile.getParents();
            parents.append(QPoint(current_x, current_y));

            m_weight_map[idx_x][idx_y].setParent(parents);

            priority_queue.push(m_weight_map[idx_x][idx_y]);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(idx_x, idx_y, TileType::Visited);
            }
        }
    }
}
