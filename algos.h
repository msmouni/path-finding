#ifndef ALGOS_H
#define ALGOS_H

#include <cstdint>
#include <QString>

enum class PathFindingAlgos : uint8_t
{
    None = 0,
    Bfs = 1,
    Dijkstra = 2,
    Astar = 3,
};

const uint8_t NB_ALGOS = 3;

PathFindingAlgos getAlgoFromInt(int algo);

QString getAlgoName(int algo);

#endif // ALGOS_H
