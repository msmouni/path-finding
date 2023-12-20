#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include <functional>
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

    void processAdjacentTiles(QPoint tile_idx, std::function<void(int, int)> process); // std::function is a general-purpose polymorphic function wrapper (or maybe use function reference or function pointer)
};

#endif // PATHFINDING_H
