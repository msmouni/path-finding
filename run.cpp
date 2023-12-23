#include "run.h"

PathFindingRunner::PathFindingRunner(Map *map)

{
    // QObject::moveToThread: Cannot move objects with a parent
    m_bfs = new Bfs(nullptr, map);
    m_dijstra = new Dijkstra(nullptr, map);
    m_a_star = new Astar(nullptr, map);
}

void PathFindingRunner::run()
{
    //     m_bfs->find();
    //    m_dijstra->find();
    m_a_star->find();
}

void PathFindingRunner::restart()
{
    if (isRunning())
    {
        terminate();
        wait();
    }
    start();
}
