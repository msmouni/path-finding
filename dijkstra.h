#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "find.h"
#include <queue>

struct DijkstraTile
{
private:
    PathFindingTile m_tile;
    qreal m_weight;

public:
    DijkstraTile(QPoint idx, qreal weight, PathFindingTile &parent) : m_weight(weight), m_tile(idx, parent){};

    DijkstraTile(QPoint idx, qreal weight) : m_weight(weight), m_tile(idx){};

    const QPoint &getPos() const
    {
        return m_tile.getPos();
    }
    qreal getWeight() const
    {
        return m_weight;
    }

    const PathFindingTile &getTile() const
    {
        return m_tile;
    }

    void setWeight(qreal new_weight)
    {
        m_weight = new_weight;
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
    QVector<QVector<qreal>> m_weight_map;

    // Note reg std::multiset : https://stackoverflow.com/questions/5895792/why-is-using-a-stdmultiset-as-a-priority-queue-faster-than-using-a-stdpriori
    std::priority_queue<DijkstraTile> m_priority_queue;

    void reinitWeightMap();

    void reset();
    void processTile(const int &tile_idx_x, const int &tile_idx_y, MvmtDirection mvmt_dir);
};

#endif // DIJKSTRA_H
