#ifndef BFS_H
#define BFS_H

#include "find.h"

class Bfs : public PathFinding
{
public:
    explicit Bfs(QObject *parent = nullptr, Map *map = nullptr);

    void find();

private:
    void addTileToBfsQueue(int idx_x, int idx_y, QQueue<QVector<QPoint>> &queue, QVector<QPoint> &current_parents);
};

#endif // BFS_H
