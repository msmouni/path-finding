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
    reinit();

    qint64 duration = 0;

    QPoint start_idx = m_map->getStartIdx();
    m_weight_map[start_idx.x()][start_idx.y()].setWeight(0);

    m_priority_queue.push(m_weight_map[start_idx.x()][start_idx.y()]);

    while (!m_priority_queue.empty())
    {
        const DijkstraTile &current_tile = m_priority_queue.top();

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

        processAdjacentTiles(current_tile.getIdx());

        m_priority_queue.pop();

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

void Dijkstra::reinit()
{
    reinitWeightMap();

    m_timer.restart();

    while (!m_priority_queue.empty())
    {
        m_priority_queue.pop();
    }
}

void Dijkstra::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    // addDijkstraTile
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {
            const DijkstraTile &current_tile = m_priority_queue.top();

            const QPoint &current_tile_idx = current_tile.getIdx();
            int current_x = current_tile_idx.x();
            int current_y = current_tile_idx.y();

            qreal weight = sqrt(pow(current_x - tile_idx_x, 2) + pow(current_y - tile_idx_y, 2)) + current_tile.getWeight();

            //            qDebug()<<weight;

            if (m_weight_map[tile_idx_x][tile_idx_y].getWeight() > weight)
            {
                m_weight_map[tile_idx_x][tile_idx_y].setWeight(weight);
            }

            QVector<QPoint> parents = current_tile.getParents();
            parents.append(QPoint(current_x, current_y));

            m_weight_map[tile_idx_x][tile_idx_y].setParent(parents);

            m_priority_queue.push(m_weight_map[tile_idx_x][tile_idx_y]);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
            }
        }
    }
}
