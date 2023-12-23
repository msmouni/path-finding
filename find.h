#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include "map.h"

class PathFinding : public QObject
{
    Q_OBJECT
public:
    explicit PathFinding(QObject *parent = nullptr, Map *map = nullptr);

    virtual void find() = 0;

protected:
    Map *m_map;
    QElapsedTimer m_timer;

    virtual void processTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    virtual void reinit() = 0;
    void processAdjacentTiles(const QPoint &tile_idx);
};

#endif // PATHFINDING_H
