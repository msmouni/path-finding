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

        m_queue.enqueue(PathFindingTile(m_map->getStartIdx()));

        bool found_path = false;

        while (!m_queue.isEmpty())
        {
            total_checks += 1;

            m_current_tile = m_queue.dequeue();

            TileType tile_type = m_map->getTileType(m_current_tile.getPos());

            if (tile_type == TileType::Target)
            {
                //            qDebug()<<current_parents;
                qDebug() << "elapsed time" << duration << " us";
                for (QPoint tile_pos : m_current_tile.getParents())
                {
                    TileType tile_type = m_map->getTileType(tile_pos);
                    if (tile_type != TileType::Target && tile_type != TileType::Start)
                    {
                        m_map->setTileType(tile_pos, TileType::Path);

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

            processAdjacentTiles(m_current_tile.getPos());

            duration += m_timer.nsecsElapsed() / 1000;

            m_map->update();

            QThread::msleep(m_visual_delay_ms);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(m_current_tile.getPos(), tile_type);

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

void Bfs::reset()
{
    m_map->clearVisited();

    m_queue.clear();
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
            m_queue.enqueue(PathFindingTile(QPoint(tile_idx_x, tile_idx_y), m_current_tile));

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
            }
        }
    }
}
