#include "bfs.h"
#include <QQueue>
#include <QThread>

Bfs::Bfs(QObject *parent, Map *map)
    : PathFinding{parent, map}
{
}

void Bfs::find()
{
    m_map->clearVisited();

    //    qDebug() << "Running Bfs in Thread:" << QThread::currentThreadId();

    m_timer.restart();

    qint64 duration = 0;

    QQueue<QVector<QPoint>> queue;

    QVector<QPoint> start;
    start.append(m_map->getStartIdx());

    queue.enqueue(start);

    bool found_path = false;

    while (!queue.isEmpty())
    {
        QVector<QPoint> current_parents = queue.dequeue();
        QPoint current_cell = current_parents.last();

        TileType tile_type = m_map->getTileType(current_cell);

        if (tile_type == TileType::Target)
        {
            //            qDebug()<<current_parents;
            qDebug() << "elapsed time" << duration << " us";
            for (QPoint tile_pos : current_parents)
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

        /*for (QPoint &adjacent_tile: getAdjacentTiles(current_cell)){
            addTileToBfsQueue(adjacent_tile.x(), adjacent_tile.y(), queue, current_parents);
        }*/

        // NOTE: Still slower than the next block
        processAdjacentTiles(current_cell, [this, &queue, &current_parents](int x, int y)
                             {
            // Lambda implementation
            addTileToBfsQueue(x, y, queue, current_parents); });

        /*addTileToBfsQueue(current_cell.x() - 1, current_cell.y(), queue, current_parents);
        addTileToBfsQueue(current_cell.x() + 1, current_cell.y(), queue, current_parents);
        addTileToBfsQueue(current_cell.x(), current_cell.y() - 1, queue, current_parents);
        addTileToBfsQueue(current_cell.x(), current_cell.y() + 1, queue, current_parents);
        // Note: since we are using a queue the order of insertion matters (First being inserted, first to be checked), so we prioritize horizantal and vertical mvmt over diagonal ones
        // TODO: factorize
        if (current_cell.x() > 0 && m_map->getTileType(current_cell.x() - 1, current_cell.y()) != TileType::Solid && current_cell.y() > 0 && m_map->getTileType(current_cell.x(), current_cell.y() - 1) != TileType::Solid)
        {
            addTileToBfsQueue(current_cell.x() - 1, current_cell.y() - 1, queue, current_parents);
        }
        if (current_cell.x() > 0 && m_map->getTileType(current_cell.x() - 1, current_cell.y()) != TileType::Solid && current_cell.y() < m_map->getNbRows() - 1 && m_map->getTileType(current_cell.x(), current_cell.y() + 1) != TileType::Solid)
        {
            addTileToBfsQueue(current_cell.x() - 1, current_cell.y() + 1, queue, current_parents);
        }
        if (current_cell.x() < m_map->getNbColumns() - 1 && m_map->getTileType(current_cell.x() + 1, current_cell.y()) != TileType::Solid && current_cell.y() > 0 && m_map->getTileType(current_cell.x(), current_cell.y() - 1) != TileType::Solid)
        {
            addTileToBfsQueue(current_cell.x() + 1, current_cell.y() - 1, queue, current_parents);
        }
        if (current_cell.x() < m_map->getNbColumns() - 1 && m_map->getTileType(current_cell.x() + 1, current_cell.y()) != TileType::Solid && current_cell.y() < m_map->getNbRows() - 1 && m_map->getTileType(current_cell.x(), current_cell.y() + 1) != TileType::Solid)
        {
            addTileToBfsQueue(current_cell.x() + 1, current_cell.y() + 1, queue, current_parents);
        }*/
        duration += m_timer.nsecsElapsed() / 1000;
        m_map->update();

        QThread::msleep(5);

        m_timer.restart();
    }
}

void Bfs::addTileToBfsQueue(int idx_x, int idx_y, QQueue<QVector<QPoint>> &queue, QVector<QPoint> &current_parents)
{
    if (0 <= idx_x && idx_x < m_map->getNbColumns() && 0 <= idx_y && idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(idx_x, idx_y);
        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {

            QVector<QPoint> parents = current_parents;
            parents.append(QPoint(idx_x, idx_y));
            queue.enqueue(parents);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(idx_x, idx_y, TileType::Visited);
            }
        }
    }
}
