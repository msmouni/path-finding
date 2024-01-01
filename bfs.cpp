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

PathFindingResult Bfs::find()
{
    //    qDebug() << "Running Bfs in Thread:" << QThread::currentThreadId();

    qint64 duration = 0;
    int total_checks = 0;

    if (m_map->isReady())
    {
        reset();

        QVector<QPoint> start;
        start.append(m_map->getStartIdx());

        m_queue.enqueue(start);

        bool found_path = false;

        while (!m_queue.isEmpty())
        {
            total_checks += 1;

            m_current_parents = m_queue.dequeue();
            QPoint &current_tile = m_current_parents.last();

            TileType tile_type = m_map->getTileType(current_tile);

            if (tile_type == TileType::Target)
            {
                //            qDebug()<<current_parents;
                qDebug() << "elapsed time" << duration << " us";
                for (QPoint tile_pos : m_current_parents)
                {
                    TileType tile_type = m_map->getTileType(tile_pos);
                    if (tile_type != TileType::Target && tile_type != TileType::Start)
                    {
                        m_map->setTileType(tile_pos, TileType::Path);

                        QThread::msleep(m_visual_delay_ms);

                        m_map->update();
                    }
                }

                return PathFindingResult(true, total_checks, duration, m_current_parents);
            }
            else
            {
                // Not part of the algorithm, just for visualization
                duration += m_timer.nsecsElapsed() / 1000;
                m_map->setTileType(current_tile, TileType::Current);

                // TMP
                for (QPoint parent : m_current_parents)
                {
                    if (m_map->getTileType(parent) != TileType::Start)
                    {
                        m_map->setTileType(parent, TileType::Current);
                    }
                }

                m_timer.restart();
            }

            processAdjacentTiles(current_tile);

            duration += m_timer.nsecsElapsed() / 1000;

            m_map->update();

            QThread::msleep(m_visual_delay_ms);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(current_tile, tile_type);

                // TMP
                for (QPoint parent : m_current_parents)
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
