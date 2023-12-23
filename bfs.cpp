#include "bfs.h"
#include <QThread>

Bfs::Bfs(QObject *parent, Map *map)
    : PathFinding{parent, map}
{
    init();
}

void Bfs::init()
{
    reset();
}


void Bfs::find()
{
    //    qDebug() << "Running Bfs in Thread:" << QThread::currentThreadId();

    reset();

    qint64 duration = 0;

    QVector<QPoint> start;
    start.append(m_map->getStartIdx());

    m_queue.enqueue(start);

    bool found_path = false;

    while (!m_queue.isEmpty())
    {
        m_current_parents = m_queue.dequeue();
        QPoint &current_cell = m_current_parents.last();

        TileType tile_type = m_map->getTileType(current_cell);

        if (tile_type == TileType::Target)
        {
            //            qDebug()<<current_parents;
            qDebug() << "elapsed time" << duration << " us";
            for (QPoint tile_pos : m_current_parents)
            {
                TileType tile_type = m_map->getTileType(tile_pos);
                if (tile_type != TileType::Target && tile_type != TileType::Start)
                {
                    m_map->setTileType(tile_pos.x(), tile_pos.y(), TileType::Path);

                    QThread::msleep(m_visual_delay_ms);

                    m_map->update();
                }
            }

            return;
        }

        processAdjacentTiles(current_cell);

        duration += m_timer.nsecsElapsed() / 1000;
        m_map->update();

        QThread::msleep(m_visual_delay_ms);

        m_timer.restart();
    }
}


void Bfs::reset()
{
    m_map->clearVisited();

    m_queue.clear();
    m_current_parents.clear();
    m_timer.restart();
}

void Bfs::processTile(const int &tile_idx_x, const int &tile_idx_y)
{
    // addTileToBfsQueue
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {

            QVector<QPoint> parents = m_current_parents;
            parents.append(QPoint(tile_idx_x, tile_idx_y));
            m_queue.enqueue(parents);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
            }
        }
    }
}
