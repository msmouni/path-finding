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

    const QPoint &getIdx() const
    {
        return m_idx;
    }
    qreal getWeight() const
    {
        return m_weight;
    }

    void setWeight(qreal new_weight)
    {
        m_weight = new_weight;
    }

    const QVector<QPoint> &getParents() const
    {
        return m_parents;
    }

    void setParent(QVector<QPoint> new_parents)
    {
        m_parents = new_parents;
    }

    void reset(qreal new_weight)
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

    void init();
    PathFindingResult find();

private:
    const qreal MAX_WEIGHT_VALUE = 99999;
    QVector<QVector<DijkstraTile>> m_weight_map;

    // Note reg std::multiset : https://stackoverflow.com/questions/5895792/why-is-using-a-stdmultiset-as-a-priority-queue-faster-than-using-a-stdpriori
    std::priority_queue<DijkstraTile> m_priority_queue;
    DijkstraTile m_current_tile;

    void reinitWeightMap();

    void reset();
    void processTile(const int &tile_idx_x, const int &tile_idx_y, MvmtDirection mvmt_dir);
};

#endif // DIJKSTRA_H
