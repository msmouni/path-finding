#include "run.h"

PathFindingRunner::PathFindingRunner(Map *map)

{
    // QObject::moveToThread: Cannot move objects with a parent
    m_bfs = new Bfs(nullptr, map);
}

void PathFindingRunner::run()
{
    m_bfs->find();
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
