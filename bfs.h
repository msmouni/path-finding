#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

class Bfs : public PathFinding
{
public:
    explicit Bfs(QObject *parent = nullptr, Map *map = nullptr);

    void init();

private:
    QVector<QVector<bool>> m_queued_map;
    QQueue<PathFindingTile> m_queue;

    // from parent
    void reset();
    void initSearch();
    void updateCurrentTile();
    bool isQueueEmpty();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);

    void reinitQueuedMap();
};

#endif // BFS_H
