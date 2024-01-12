#ifndef BFS_H
#define BFS_H

#include "find.h"
#include <QQueue>

struct BfsVisit
{
private:
    bool m_visited;
    int m_jump_count;

public:
    BfsVisit(bool visited = false, int jump_count = MAX_JUMP_COUNT) : m_visited(visited), m_jump_count(jump_count) {}

    bool isVisited()
    {
        return m_visited;
    }
    void setVisited(bool val)
    {
        m_visited = val;
    }

    int getJumpCount()
    {
        return m_jump_count;
    }
    void setJumpCount(int val)
    {
        m_jump_count = val;
    }
};

class Bfs : public PathFinding
{
public:
    explicit Bfs(QObject *parent = nullptr, Map *map = nullptr);

    void init();

private:
    QVector<QVector<BfsVisit>> m_queued_map;
    QQueue<PathFindingTile> m_queue;

    // from parent
    void reset();
    void initSearch();
    void updateCurrentTile();
    bool isQueueEmpty();
    void processTile(const int &tile_idx_x, const int &tile_idx_y);
    void processJumpTile(const int &tile_idx_x, const int &tile_idx_y);

    void reinitQueuedMap();
};

#endif // BFS_H
