#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include "map.h"

class PathFinding : public QObject
{
    Q_OBJECT
public:
    explicit PathFinding(QObject *parent = nullptr, Map *map = nullptr);

    void find();

protected:
    Map *m_map;

    void Bfs();
    void addTileToBfsQueue(int idx_x, int idx_y, QQueue<QVector<QPoint>> &queue, QVector<QPoint> &current_parents);
};

#endif // PATHFINDING_H
