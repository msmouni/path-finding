#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

class Bfs : public PathFinding
{
public:
    explicit Bfs(QObject *parent = nullptr, Map *map = nullptr);

    void find();

private:
    QQueue<QVector<QPoint>> m_queue;
    QVector<QPoint> m_current_parents;

    void reinit();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);
};

#endif // BFS_H
