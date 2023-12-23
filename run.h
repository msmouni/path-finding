#ifndef PATHFINDINGRUNNER_H
#define PATHFINDINGRUNNER_H

#include <QThread>
#include "bfs.h"
#include "dijkstra.h"
#include "a_star.h"

class PathFindingRunner : public QThread
{
    Q_OBJECT
public:
    PathFindingRunner(Map *map = nullptr);

    void run() override;

protected slots:
    void restart();

private:
    Bfs *m_bfs;
    Dijkstra *m_dijstra;
    Astar *m_a_star;
};

#endif // PATHFINDINGRUNNER_H
