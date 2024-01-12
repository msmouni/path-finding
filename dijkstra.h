#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "find.h"
#include <queue>

struct DijkstraCost
{
private:
    qreal m_cost;
    int m_nb_jump;

public:
    DijkstraCost(qreal cost = 0, int nb_jump = MAX_JUMP_COUNT) : m_cost(cost), m_nb_jump(nb_jump) {}

    void setCost(qreal new_cost)
    {
        m_cost = new_cost;
    }

    qreal getCost() const
    {
        return m_cost;
    }

    void setJumpCost(qreal new_cost, int nb_jump)
    {
        m_cost = new_cost;
        m_nb_jump = nb_jump;
    }

    int getNbJump() const
    {
        return m_nb_jump;
    }
};

struct DijkstraTile
{
private:
    PathFindingTile m_tile;
    qreal m_weight;

public:
    DijkstraTile(QPoint pos, qreal weight, PathFindingTile &parent) : m_weight(weight), m_tile(pos, parent) {}

    DijkstraTile(QPoint pos = QPoint(0, 0), qreal weight = 0) : m_weight(weight), m_tile(pos) {}

    const QPoint &getPos() const
    {
        return m_tile.getPos();
    }

    const PathFindingTile &getTile() const
    {
        return m_tile;
    }

    const qreal &getWeight() const
    {
        return m_weight;
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

private:
    const qreal MAX_WEIGHT_VALUE = 99999;
    QVector<QVector<DijkstraCost>> m_weight_map;
    qreal m_current_tile_weight;

    // Note reg std::multiset : https://stackoverflow.com/questions/5895792/why-is-using-a-stdmultiset-as-a-priority-queue-faster-than-using-a-stdpriori
    std::priority_queue<DijkstraTile> m_priority_queue;

    // from parent
    void reset();
    void initSearch();
    void updateCurrentTile();
    bool isQueueEmpty();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);
    void processJumpTile(const int &tile_idx_x, const int &tile_idx_y);

    void reinitWeightMap();
};

#endif // DIJKSTRA_H
