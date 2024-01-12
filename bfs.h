#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

struct BfsTile{
private:
    MvtToPoint m_mvmt_point;
    QVector<MvtToPoint> m_parents;
    int m_jump_count;
public:
    BfsTile(MvtToPoint mvmt_point = MvtToPoint(QPoint(0, 0)), QVector<MvtToPoint> parents= QVector<MvtToPoint>()): m_mvmt_point(mvmt_point), m_parents(parents){
        m_jump_count=0;

        MvmtDirection mvmt_dir=mvmt_point.getPrevMvmtDir();

        if (mvmt_dir == MvmtDirection::Top || mvmt_dir == MvmtDirection::TopLeft || mvmt_dir == MvmtDirection::TopRight){
            m_jump_count+=1;
        }

        for (MvtToPoint parent: m_parents){
            MvmtDirection mvmt_dir=parent.getPrevMvmtDir();
            if (mvmt_dir == MvmtDirection::Top || mvmt_dir == MvmtDirection::TopLeft || mvmt_dir == MvmtDirection::TopRight){
                m_jump_count+=1;
            }
        }
    };

    QPoint& getPoint(){
        return m_mvmt_point.getPoint();
    }

    MvtToPoint& getMvtToPoint(){
        return m_mvmt_point;
    }

    QVector<MvtToPoint>& getParents(){
        return m_parents;
    }

    // TMP
    QVector<QPoint> getParentsPoints(){
        QVector<QPoint> res;
        for (MvtToPoint mvmt_point:m_parents){
            res.append(mvmt_point.getPoint());
        }
        return res;
    }

    void resetJumpCount(){
        m_jump_count=0;
    }

    const int& getJumpCount(){
        return m_jump_count;
    }

};

class Bfs : public PathFinding
{
public:
    explicit Bfs(QObject *parent = nullptr, Map *map = nullptr);

    void init();
    PathFindingResult find();

private:
    QQueue<BfsTile> m_queue;
    BfsTile m_current_tile;
//    QVector<BfsTile> m_current_parents;
//    int m_jump_count;
    bool m_landed;

    void reset();
    void processTile(const int &tile_idx_x, const int &tile_idx_y, MvmtDirection mvmt_dir);
};

#endif // BFS_H
