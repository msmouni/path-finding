#ifndef PATHFINDINGRUNNER_H
#define PATHFINDINGRUNNER_H

#include "find.h"
#include <QThread>
#include "bfs.h"

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
};

#endif // PATHFINDINGRUNNER_H
