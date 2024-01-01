#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

struct BfsTile
{
private:
    MvtToPoint m_mvmt_point;
    QVector<MvtToPoint> m_parents;
    int m_jump_count;

public:
    BfsTile(MvtToPoint mvmt_point = MvtToPoint(QPoint(0, 0))) : m_mvmt_point(mvmt_point)
    {
        m_jump_count = 0;

        MvmtDirection mvmt_dir = mvmt_point.getPrevMvmtDir();

        if (mvmt_dir == MvmtDirection::Top || mvmt_dir == MvmtDirection::TopLeft || mvmt_dir == MvmtDirection::TopRight)
        {
            m_jump_count += 1;
        }
    };

    BfsTile(BfsTile &parent_tile, MvtToPoint mvmt_point) : m_mvmt_point(mvmt_point)
    {
        m_parents = parent_tile.getParents();

        m_parents.append(parent_tile.getMvtToPoint());

        m_jump_count = parent_tile.getJumpCount();

        MvmtDirection mvmt_dir = mvmt_point.getPrevMvmtDir();
        if (mvmt_dir == MvmtDirection::Top || mvmt_dir == MvmtDirection::TopLeft || mvmt_dir == MvmtDirection::TopRight)
        {
            m_jump_count += 1;
        }
    };

    QPoint &getPoint()
    {
        return m_mvmt_point.getPoint();
    }

    MvtToPoint &getMvtToPoint()
    {
        return m_mvmt_point;
    }

    QVector<MvtToPoint> &getParents()
    {
        return m_parents;
    }

    // TMP
    QVector<QPoint> getParentsPoints()
    {
        QVector<QPoint> res;
        for (MvtToPoint mvmt_point : m_parents)
        {
            res.append(mvmt_point.getPoint());
        }
        return res;
    }

    void resetJumpCount()
    {
        qDebug() << "Reset Count";
        m_jump_count = 0;
    }

    const int &getJumpCount()
    {
        return m_jump_count;
    }

    const MvmtDirection &getPrevMvmtDir()
    {
        return m_parents.last().getPrevMvmtDir();
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
