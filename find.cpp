#include "find.h"
#include <QQueue>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>

PathFinding::PathFinding(QObject *parent, Map *map)
    : QObject{parent}, m_map(map)
{
    m_timer.start();
}

void PathFinding::processAdjacentTiles(QPoint tile_idx, std::function<void(int, int)> process)
{
    int tile_x = tile_idx.x();
    int tile_y = tile_idx.y();

    process(tile_x - 1, tile_y);
    process(tile_x + 1, tile_y);
    process(tile_x, tile_y - 1);
    process(tile_x, tile_y + 1);

    // Note: since we are using a queue the order of insertion matters (First being inserted, first to be checked), so we prioritize horizantal and vertical mvmt over diagonal ones
    int map_rows_nb = m_map->getNbRows();
    int map_columns_nb = m_map->getNbColumns();

    if (tile_x > 0 && m_map->getTileType(tile_x - 1, tile_y) != TileType::Solid && tile_y > 0 && m_map->getTileType(tile_x, tile_y - 1) != TileType::Solid)
    {
        process(tile_x - 1, tile_y - 1);
    }
    if (tile_x > 0 && m_map->getTileType(tile_x - 1, tile_y) != TileType::Solid && tile_y < map_rows_nb - 1 && m_map->getTileType(tile_x, tile_y + 1) != TileType::Solid)
    {
        process(tile_x - 1, tile_y + 1);
    }
    if (tile_x < map_columns_nb - 1 && m_map->getTileType(tile_x + 1, tile_y) != TileType::Solid && tile_y > 0 && m_map->getTileType(tile_x, tile_y - 1) != TileType::Solid)
    {
        process(tile_x + 1, tile_y - 1);
    }
    if (tile_x < map_columns_nb - 1 && m_map->getTileType(tile_x + 1, tile_y) != TileType::Solid && tile_y < map_rows_nb - 1 && m_map->getTileType(tile_x, tile_y + 1) != TileType::Solid)
    {
        process(tile_x + 1, tile_y + 1);
    }
}
