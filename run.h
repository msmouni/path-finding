#ifndef PATHFINDINGRUNNER_H
#define PATHFINDINGRUNNER_H

#include "find.h"
#include <QThread>

class PathFindingRunner : public QThread
{
    Q_OBJECT
public:
    PathFindingRunner(Map *map = nullptr);

    void run() override;

protected slots:
    void restart();

private:
    PathFinding *m_path_finding;
};

#endif // PATHFINDINGRUNNER_H
