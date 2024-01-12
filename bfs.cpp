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

        //        QVector<BfsTile> start;
        //        start.append();

        m_queue.enqueue(BfsTile(MvtToPoint(m_map->getStartIdx())));

        bool found_path = false;

        while (!m_queue.isEmpty())
        {
            total_checks+=1;

            m_current_tile = m_queue.dequeue();
            //            QPoint &current_tile = m_current_parents.last().getPoint();

            if  (m_current_tile.getPoint().y() <m_map->getNbRows() - 1 &&
                m_map->getTileType(m_current_tile.getPoint().x(), m_current_tile.getPoint().y() + 1) == TileType::Solid){
                m_current_tile.resetJumpCount();
                m_landed=true;

            }

            TileType tile_type = m_map->getTileType(m_current_tile.getPoint());

            if (tile_type == TileType::Target)
            {
                //            qDebug()<<current_parents;
                qDebug() << "elapsed time" << duration << " us";
                for (QPoint tile_pos: m_current_tile.getParentsPoints())
                {
                    TileType tile_type = m_map->getTileType(tile_pos);
                    if (tile_type != TileType::Target && tile_type != TileType::Start)
                    {
                        m_map->setTileType(tile_pos, TileType::Path);

                        QThread::msleep(m_visual_delay_ms);

                        m_map->update();
                    }
                }

                return PathFindingResult(true, total_checks, duration, m_current_tile.getParentsPoints());
            }
            else
            {
                // Not part of the algorithm, just for visualization
                duration += m_timer.nsecsElapsed() / 1000;
                m_map->setTileType(m_current_tile.getPoint(), TileType::Current);

                // TMP
                for (QPoint parent: m_current_tile.getParentsPoints()){
                    if (m_map->getTileType(parent) != TileType::Start)
                    {
                        m_map->setTileType(parent, TileType::Current);
                    }
                }


                m_timer.restart();
            }

            processAdjacentTiles(m_current_tile.getPoint());

            duration += m_timer.nsecsElapsed() / 1000;

            m_map->update();

            QThread::msleep(m_visual_delay_ms);

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(m_current_tile.getPoint(), tile_type);

                // TMP
                for (QPoint parent: m_current_tile.getParentsPoints()){
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

//     m_jump_count=0;
    m_landed=false;
    m_queue.clear();
    //    m_current_parents.clear();
    m_timer.restart();
}

void Bfs::processTile(const int &tile_idx_x, const int &tile_idx_y, MvmtDirection mvmt_dir)
{
    // addTileToBfsQueue
    if (0 <= tile_idx_x && tile_idx_x < m_map->getNbColumns() && 0 <= tile_idx_y && tile_idx_y < m_map->getNbRows())
    {
        TileType tile_type = m_map->getTileType(tile_idx_x, tile_idx_y);

        if (tile_type == TileType::Empty || tile_type == TileType::Target)
        {

            QVector<MvtToPoint> parents = m_current_tile.getParents();
            parents.append(m_current_tile.getMvtToPoint());

            BfsTile bfs_tile=BfsTile(MvtToPoint(QPoint(tile_idx_x, tile_idx_y), mvmt_dir), parents);

            if (mvmt_dir == MvmtDirection::Top || mvmt_dir == MvmtDirection::TopLeft || mvmt_dir == MvmtDirection::TopRight){
//                int up_count=0;

                /*if (parents.length()>=4){
                    for (MvtToPoint parent_mvt_pnt:parents.last(4)){
                        MvmtDirection parent_mvt=parent_mvt_pnt.getPrevMvmtDir();
                        if (parent_mvt == MvmtDirection::Top || parent_mvt == MvmtDirection::TopLeft || parent_mvt == MvmtDirection::TopRight){
                            up_count+=1;
                        }
                    }
                }*/

                if (mvmt_dir == MvmtDirection::TopLeft){
                    qDebug()<<"TopLeft";
                }
                qDebug()<<"jump_count"<<bfs_tile.getJumpCount();

                /*if  (m_current_tile.getPoint().y() <m_map->getNbRows() - 1 &&
                    m_map->getTileType(m_current_tile.getPoint().x(), m_current_tile.getPoint().y() + 1) == TileType::Solid){
                    m_jump_count=0;

                }*/

                if (m_landed && bfs_tile.getJumpCount()<=3){
//                    m_jump_count+=1;

                    m_queue.enqueue(bfs_tile);

                    if (tile_type != TileType::Target)
                    {
                        m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
                    }
                }
            }else {
                m_queue.enqueue(bfs_tile);

                if (tile_type != TileType::Target)
                {
                    m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
                }
            }
            /*m_queue.enqueue(BfsTile(MvtToPoint(QPoint(tile_idx_x, tile_idx_y), mvmt_dir), parents));

            if (tile_type != TileType::Target)
            {
                m_map->setTileType(tile_idx_x, tile_idx_y, TileType::Visited);
            }*/
        }
    }
}
