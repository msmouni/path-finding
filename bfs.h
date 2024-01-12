#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

struct BfsTile{
private:
    MvtToPoint m_mvmt_point;
    QVector<MvtToPoint> m_parents;
public:
    BfsTile(MvtToPoint mvmt_point = MvtToPoint(QPoint(0, 0)), QVector<MvtToPoint> parents= QVector<MvtToPoint>()): m_mvmt_point(mvmt_point), m_parents(parents){};

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

    void reset();
    void processTile(const int &tile_idx_x, const int &tile_idx_y, MvmtDirection mvmt_dir);
};

#endif // BFS_H
