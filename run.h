#ifndef PATHFINDINGRUNNER_H
#define PATHFINDINGRUNNER_H

#include <QThread>
#include "algos.h"
#include "bfs.h"
#include "dijkstra.h"
#include "a_star.h"

struct RunResult
{
    PathFindingAlgos m_algo;
    PathFindingResult m_path_finding;

    RunResult(PathFindingAlgos algo = PathFindingAlgos::None, PathFindingResult path_finding = {0}) : m_algo(algo), m_path_finding(path_finding) {}
};

class PathFindingRunner : public QThread
{
    Q_OBJECT
public:
    PathFindingRunner(Map *map = nullptr);

    void run() override;

    void restart();
    void setVisualDelayMs(int delay_ms);
    void setPlatformer(bool);
    bool isReady();

public slots:
    void reinit();

protected slots:
    void setAlgo(int algo_int);

signals:
    void pathFindingRes(RunResult);

private:
    PathFindingAlgos m_selected_algo;
    Bfs *m_bfs;
    Dijkstra *m_dijstra;
    Astar *m_a_star;
};

#endif // PATHFINDINGRUNNER_H
