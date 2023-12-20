#ifndef ASTAR_H
#define ASTAR_H

#include "find.h"
#include <queue>

struct AstarTile
{
private:
    qreal m_cost;
    qreal m_estimated_target_cost;
    QPoint m_idx;
    QVector<QPoint> m_parents;

public:
    AstarTile(qreal cost = 0, qreal target_cost = 0, QPoint idx = QPoint(0, 0), QVector<QPoint> parents = QVector<QPoint>()) : m_cost(cost), m_estimated_target_cost(target_cost), m_idx(idx), m_parents(parents){};

    QPoint &getIdx()
    {
        return m_idx;
    }
    qreal getWeight() const
    {
        return m_cost + m_estimated_target_cost;
    }

    void setCost(qreal new_cost)
    {
        m_cost = new_cost;
    }

    qreal getCost()
    {
        return m_cost;
    }

    void setTargetCost(qreal new_target_cost)
    {
        m_estimated_target_cost = new_target_cost;
    }

    QVector<QPoint> getParents()
    {
        return m_parents;
    }

    void setParent(QVector<QPoint> new_parents)
    {
        m_parents = new_parents;
    }

    void reinit(qreal new_cost, qreal new_target_cost)
    {
        m_cost = new_cost;
        m_estimated_target_cost = new_target_cost;
        m_parents.clear();
    }

    // operator used in priority_queue
    bool operator<(const AstarTile &other) const
    {
        return getWeight() > other.getWeight();
    }
};

class Astar : public PathFinding
{
public:
    explicit Astar(QObject *parent = nullptr, Map *map = nullptr);

    void find();

private:
    const qreal MAX_WEIGHT_VALUE = 99999;
    QVector<QVector<AstarTile>> m_weight_map;

    void reinitWeightMap();

    qreal getEstimatedTargetCost(QPoint idx);
    qreal getEstimatedTargetCost(int &idx_x, int &idx_y);

    void addAstarTile(int idx_x, int idx_y, std::priority_queue<AstarTile> &priority_queue, AstarTile &current_tile);
};

#endif // ASTAR_H
