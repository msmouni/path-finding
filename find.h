#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include "map.h"

class PathFinding : public QObject
{
public:
    explicit PathFinding(QObject *parent = nullptr, Map *map = nullptr, int visual_delay_ms = 0);

    virtual void init() = 0;
    virtual void find() = 0;

    void setVisualDelayMs(int delay_ms);

protected:
    Map *m_map;
    QElapsedTimer m_timer;
    int m_visual_delay_ms;

    virtual void processTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    virtual void reset() = 0;
    void processAdjacentTiles(const QPoint &tile_idx);
};

#endif // PATHFINDING_H
