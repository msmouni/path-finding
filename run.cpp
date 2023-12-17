#include "run.h"

PathFindingRunner::PathFindingRunner(Map *map)

{
    // QObject::moveToThread: Cannot move objects with a parent
    m_path_finding = new PathFinding(nullptr, map);
}

void PathFindingRunner::run()
{
    m_path_finding->find();
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
