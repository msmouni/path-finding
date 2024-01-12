#include "run.h"

PathFindingRunner::PathFindingRunner(Map *map)

{
    // QObject::moveToThread: Cannot move objects with a parent
    m_bfs = new Bfs(nullptr, map);
    m_dijstra = new Dijkstra(nullptr, map);
    m_a_star = new Astar(nullptr, map);

    m_selected_algo = PathFindingAlgos::None;
}

void PathFindingRunner::run()
{
    switch (m_selected_algo)
    {
    case PathFindingAlgos::Bfs:
    {
        PathFindingResult res = m_bfs->find();
        emit pathFindingRes(RunResult(PathFindingAlgos::Bfs, res));
        break;
    }
    case PathFindingAlgos::Dijkstra:
    {
        PathFindingResult res = m_dijstra->find();
        emit pathFindingRes(RunResult(PathFindingAlgos::Dijkstra, res));
        break;
    }
    case PathFindingAlgos::Astar:
    {
        PathFindingResult res = m_a_star->find();
        emit pathFindingRes(RunResult(PathFindingAlgos::Astar, res));
        break;
    }
    default:
        break;
    }
}

void PathFindingRunner::setVisualDelayMs(int delay_ms)
{
    m_bfs->setVisualDelayMs(delay_ms);
    m_dijstra->setVisualDelayMs(delay_ms);
    m_a_star->setVisualDelayMs(delay_ms);
}

void PathFindingRunner::reinit()
{
    switch (m_selected_algo)
    {
    case PathFindingAlgos::Bfs:
    {
        m_bfs->init();
        break;
    }
    case PathFindingAlgos::Dijkstra:
    {
        m_dijstra->init();
        break;
    }
    case PathFindingAlgos::Astar:
    {
        m_a_star->init();
        break;
    }
    default:
        break;
    }
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

void PathFindingRunner::setAlgo(int algo_int)
{
    if (isRunning())
    {
        terminate();
        wait();
    }

    m_selected_algo = getAlgoFromInt(algo_int);

    reinit();

    start();
}

void PathFindingRunner::setPlatformer(bool set)
{
    m_bfs->setPlatformer(set);
    m_dijstra->setPlatformer(set);
    m_a_star->setPlatformer(set);

    restart();
}

bool PathFindingRunner::isReady()
{
    return m_selected_algo != PathFindingAlgos::None;
}
