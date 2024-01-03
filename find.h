#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include "map.h"

enum class MvmtDirection
{
    None,
    TopLeft,
    Top,
    TopRight,
    Left,
    Right,
    BottomLeft,
    Bottom,
    BottomRight
};

struct MvtToPoint
{
private:
    QPoint m_point;
    MvmtDirection m_prev_direction;

public:
    MvtToPoint(QPoint point, MvmtDirection prev_direction = MvmtDirection::None) : m_point(point), m_prev_direction(prev_direction){};

    QPoint &getPoint()
    {
        return m_point;
    }

    MvmtDirection &getPrevMvmtDir()
    {
        return m_prev_direction;
    }
};

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
    };

    PathFindingTile(QPoint pos, PathFindingTile &parent_tile) : m_pos(pos)
    {
        m_parents = parent_tile.getParents();

        m_parents.append(parent_tile.getPos());

        m_jump_count = parent_tile.getJumpCount();

        if (parent_tile.m_pos.y() > pos.y())
        {
            m_jump_count += 1;
        }
        else
        {
            m_can_jump = false;
        }
    };

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
        return m_can_jump && m_jump_count < 3;
    }

    void setCanJump()
    {
        qDebug() << "Reset Jump";
        m_jump_count = 0;
        m_can_jump = true;
    }

    const int &getJumpCount()
    {
        return m_jump_count;
    }
};

class PathFinding : public QObject
{
public:
    explicit PathFinding(QObject *parent = nullptr, Map *map = nullptr, int visual_delay_ms = 0);

    virtual void init() = 0;
    virtual PathFindingResult find() = 0;

    void setVisualDelayMs(int delay_ms);

protected:
    Map *m_map;
    QElapsedTimer m_timer;
    int m_visual_delay_ms;
    PathFindingTile m_current_tile;

    virtual void processTile(const int &tile_idx_x, const int &tile_idx_y, MvmtDirection mvmt_dir) = 0;
    virtual void reset() = 0;
    void processAdjacentTiles(const QPoint &tile_idx);
};

#endif // PATHFINDING_H
