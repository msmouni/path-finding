#ifndef FILESHANDLER_H
#define FILESHANDLER_H

#include <QVector>
#include "map.h"

class FilesHandler
{
public:
    FilesHandler();

    void saveMap(Map *map);
    void loadMap(Map *map);
};

#endif // FILESHANDLER_H
