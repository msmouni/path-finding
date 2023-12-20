#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "find.h"
#include <queue>

struct DijkstraTile
{
private:
    qreal m_weight;
    QPoint m_idx;
    QVector<QPoint> m_parents;

public:
    DijkstraTile(qreal weight = 0, QPoint idx = QPoint(0, 0), QVector<QPoint> parents = QVector<QPoint>()) : m_weight(weight), m_idx(idx), m_parents(parents){};

    QPoint &getIdx()
    {
        return m_idx;
    }
    qreal getWeight()
    {
        return m_weight;
    }

    void setWeight(qreal new_weight)
    {
        m_weight = new_weight;
    }

    QVector<QPoint> getParents()
    {
        return m_parents;
    }

    void setParent(QVector<QPoint> new_parents)
    {
        m_parents = new_parents;
    }

    void reinit(qreal new_weight)
    {
        setWeight(new_weight);
        m_parents.clear();
    }

    // operator used in priority_queue
    bool operator<(const DijkstraTile &other) const
    {
        return m_weight > other.m_weight;
    }
};

class Dijkstra : public PathFinding
{
public:
    explicit Dijkstra(QObject *parent = nullptr, Map *map = nullptr);

    void find();

private:
    const qreal MAX_WEIGHT_VALUE = 99999;
    QVector<QVector<DijkstraTile>> m_weight_map;

    void reinitWeightMap();

    void addDijkstraTile(int idx_x, int idx_y, std::priority_queue<DijkstraTile> &priority_queue, DijkstraTile &current_tile);
};

#endif // DIJKSTRA_H
