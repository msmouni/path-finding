#include "algos.h"

PathFindingAlgos getAlgoFromInt(int algo){
    switch (algo) {
    case 1:
        return PathFindingAlgos::Bfs;
    case 2:
        return PathFindingAlgos::Dijkstra;
    case 3:
        return PathFindingAlgos::Astar;
    default:
        return PathFindingAlgos::None;
    }
}

QString getAlgoName(int algo){
    switch (algo) {
    case 1:
        return "BFS";
    case 2:
        return "Dijkstra";
    case 3:
        return "A*";
    default:
        return "";
    }
}
