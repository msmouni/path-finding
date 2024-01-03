#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

class Bfs : public PathFinding
{
public:
    explicit Bfs(QObject *parent = nullptr, Map *map = nullptr);

    void init();
    PathFindingResult find();

private:
    QQueue<PathFindingTile> m_queue;

    void reset();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);
};

#endif // BFS_H
