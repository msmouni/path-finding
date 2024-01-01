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

    const QPoint &getIdx() const
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

    qreal getCost() const
    {
        return m_cost;
    }

    void setTargetCost(qreal new_target_cost)
    {
        m_estimated_target_cost = new_target_cost;
    }

    const QVector<QPoint> &getParents() const
    {
        return m_parents;
    }

    void setParent(QVector<QPoint> new_parents)
    {
        m_parents = new_parents;
    }

    void reset(qreal new_cost, qreal new_target_cost)
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

    void init();
    void find();

private:
    const qreal MAX_WEIGHT_VALUE = 99999;
    QVector<QVector<AstarTile>> m_weight_map;

    // Note reg std::multiset : https://stackoverflow.com/questions/5895792/why-is-using-a-stdmultiset-as-a-priority-queue-faster-than-using-a-stdpriori
    std::priority_queue<AstarTile> m_priority_queue;
    AstarTile m_current_tile;

    void reinitWeightMap();

    qreal getEstimatedTargetCost(const QPoint &idx);
    qreal getEstimatedTargetCost(const int &idx_x, const int &idx_y);

    void reset();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);
};

#endif // ASTAR_H
