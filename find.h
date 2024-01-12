#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include "map.h"

const int MAX_JUMP_COUNT = 3;

struct PathFindingResult
{
    bool m_path_found;
    int m_total_checks;
    int m_duration_us;
    QVector<QPoint> m_path;

    PathFindingResult(bool path_found = false, int total_checks = 0, int duration_us = 0, QVector<QPoint> path = QVector<QPoint>()) : m_path_found(path_found), m_total_checks(total_checks), m_duration_us(duration_us), m_path(path) {}

    QString toText()
    {
        QString res;
        if (m_path_found)
        {
            res = "{\nTotal checks: " + QString::number(m_total_checks) + "\nDuration: " + QString::number(m_duration_us) + " (us)\nPath Length: " + QString::number(m_path.length()) + "\n}";
        }

        return res;
    }
};

class PathFindingTile
{
private:
    QPoint m_pos;
    QVector<QPoint> m_parents;
    bool m_can_jump;
    int m_jump_count;

public:
    PathFindingTile(QPoint pos = QPoint(0, 0)) : m_pos(pos)
    {
        m_jump_count = 0;
        m_can_jump = false;
    }

    PathFindingTile(QPoint pos, PathFindingTile &parent_tile) : m_pos(pos)
    {
        m_parents = parent_tile.getPath();

        if (parent_tile.m_pos.y() > pos.y())
        {
            m_can_jump = true;
            m_jump_count = parent_tile.getJumpCount() + 1;
        }
        else
        {
            m_can_jump = false;
            m_jump_count = 0;
        }
    }

    const QPoint &getPos() const
    {
        return m_pos;
    }

    const QVector<QPoint> &getParents() const
    {
        return m_parents;
    }

    bool canJump()
    {
        return m_can_jump && m_jump_count < MAX_JUMP_COUNT;
    }

    void setCanJump()
    {
        m_jump_count = 0;
        m_can_jump = true;
    }

    const int &getJumpCount()
    {
        return m_jump_count;
    }

    bool isReachedUpWay()
    {
        return m_jump_count != 0;
    }

    QVector<QPoint> getPath()
    {
        QVector<QPoint> path = getParents();
        path.append(getPos());

        return path;
    }
};

enum class AdjacentTile
{
    Left,
    Right,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};

class PathFinding : public QObject
{
public:
    explicit PathFinding(QObject *parent = nullptr, Map *map = nullptr, int visual_delay_ms = 0);

    virtual void init() = 0;
    PathFindingResult find();

    void setVisualDelayMs(int delay_ms);
    void setPlatformer(bool);

private:
    void drawCurrentPath();
    void visualizeCurrentPath();
    void tryToProcessTile(const int &x, const int &y, AdjacentTile which_tile);
    void setCurrentTileVisited();

protected:
    Map *m_map;
    QElapsedTimer m_timer;
    qreal m_duration_us;
    int m_total_checks;
    int m_visual_delay_ms;
    PathFindingTile m_current_tile;
    bool m_platformer;

    virtual void reset() = 0;
    virtual void initSearch() = 0;
    virtual void updateCurrentTile() = 0;
    virtual bool isQueueEmpty() = 0;
    virtual void processTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    virtual void processJumpTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    void processAdjacentTiles(const QPoint &tile_idx);
};

#endif // PATHFINDING_H
