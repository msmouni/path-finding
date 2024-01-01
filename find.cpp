#include "find.h"
#include <QQueue>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>

PathFinding::PathFinding(QObject *parent, Map *map, int visual_delay_ms)
    : QObject{parent}, m_map(map), m_visual_delay_ms(visual_delay_ms)
{
    m_timer.start();
}

void PathFinding::setVisualDelayMs(int delay_ms)
{
    m_visual_delay_ms = delay_ms;
}

void PathFinding::processAdjacentTiles(const QPoint &tile_idx)
{
    int tile_x = tile_idx.x();
    int tile_y = tile_idx.y();

    processTile(tile_x - 1, tile_y);
    processTile(tile_x + 1, tile_y);
    processTile(tile_x, tile_y - 1);
    processTile(tile_x, tile_y + 1);

    // Note: since we are using a queue the order of insertion matters (First being inserted, first to be checked), so we prioritize horizantal and vertical mvmt over diagonal ones
    int map_rows_nb = m_map->getNbRows();
    int map_columns_nb = m_map->getNbColumns();

    if (tile_x > 0 && m_map->getTileType(tile_x - 1, tile_y) != TileType::Solid && tile_y > 0 && m_map->getTileType(tile_x, tile_y - 1) != TileType::Solid)
    {
        processTile(tile_x - 1, tile_y - 1);
    }
    if (tile_x > 0 && m_map->getTileType(tile_x - 1, tile_y) != TileType::Solid && tile_y < map_rows_nb - 1 && m_map->getTileType(tile_x, tile_y + 1) != TileType::Solid)
    {
        processTile(tile_x - 1, tile_y + 1);
    }
    if (tile_x < map_columns_nb - 1 && m_map->getTileType(tile_x + 1, tile_y) != TileType::Solid && tile_y > 0 && m_map->getTileType(tile_x, tile_y - 1) != TileType::Solid)
    {
        processTile(tile_x + 1, tile_y - 1);
    }
    if (tile_x < map_columns_nb - 1 && m_map->getTileType(tile_x + 1, tile_y) != TileType::Solid && tile_y < map_rows_nb - 1 && m_map->getTileType(tile_x, tile_y + 1) != TileType::Solid)
    {
        processTile(tile_x + 1, tile_y + 1);
    }
}
