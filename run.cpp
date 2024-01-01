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
    switch (m_selected_algo)
    {
    case PathFindingAlgos::Bfs:
    {
        m_bfs->find();
        break;
    }
    case PathFindingAlgos::Dijkstra:
    {
        m_dijstra->find();
        break;
    }
    case PathFindingAlgos::Astar:
    {
        m_a_star->find();
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
