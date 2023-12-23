#ifndef PATHFINDINGRUNNER_H
#define PATHFINDINGRUNNER_H

#include <QThread>
#include "algos.h"
#include "bfs.h"
#include "dijkstra.h"
#include "a_star.h"

class PathFindingRunner : public QThread
{
    Q_OBJECT
public:
    PathFindingRunner(Map *map = nullptr);

    void run() override;

    void setVisualDelayMs(int delay_ms);


protected slots:
    void reinit();
    void restart();
    void setAlgo(int algo_int);

private:
    PathFindingAlgos m_selected_algo;
    Bfs *m_bfs;
    Dijkstra *m_dijstra;
    Astar *m_a_star;
};

#endif // PATHFINDINGRUNNER_H
