#include "run.h"

PathFindingRunner::PathFindingRunner(Map *map)

{
    // QObject::moveToThread: Cannot move objects with a parent
    m_bfs = new Bfs(nullptr, map);
    m_dijstra = new Dijkstra(nullptr, map);
}

void PathFindingRunner::run()
{
    // m_bfs->find();
    m_dijstra->find();
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
