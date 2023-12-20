#ifndef PATHFINDINGRUNNER_H
#define PATHFINDINGRUNNER_H

#include <QThread>
#include "bfs.h"
#include "dijkstra.h"

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
};

#endif // PATHFINDINGRUNNER_H
