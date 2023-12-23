#ifndef ALGOS_H
#define ALGOS_H

#include <cstdint>
#include <QString>


enum class PathFindingAlgos:uint8_t{
    None =0,
    Bfs =1,
    Dijkstra =2,
    Astar =3,
};

const uint8_t NB_ALGOS=3;

PathFindingAlgos getAlgoFromInt(int algo);
//{
//    switch (algo) {
//    case 1:
//        return PathFindingAlgos::Bfs;
//    case 2:
//        return PathFindingAlgos::Dijkstra;
//    case 3:
//        return PathFindingAlgos::Astar;
//    default:
//        return PathFindingAlgos::None;
//    }
//}

QString getAlgoName(int algo);
//{
//    switch (algo) {
//    case 1:
//        return "BFS";
//    case 2:
//        return "Dijkstra";
//    case 3:
//        return "A*";
//    default:
//        return "";
//    }
//}


#endif // ALGOS_H
