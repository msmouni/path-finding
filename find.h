#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include "map.h"


struct PathFindingResult
{
    bool m_path_found;
    int m_total_checks;
    int m_duration_us;
    QVector<QPoint> m_path;

    PathFindingResult(bool path_found= false,int total_checks=0, int duration_us=0, QVector<QPoint> path= QVector<QPoint>()): m_path_found(path_found),m_total_checks(total_checks), m_duration_us(duration_us), m_path(path){}

    QString toText(){
        QString res;
        if (m_path_found){
            res= "{\nTotal checks: " + QString::number(m_total_checks) + "\nDuration: " + QString::number(m_duration_us) + " (us)\nPath Length: " + QString::number(m_path.length()) +"\n}";
        }

        return res;
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

    virtual void processTile(const int &tile_idx_x, const int &tile_idx_y) = 0;
    virtual void reset() = 0;
    void processAdjacentTiles(const QPoint &tile_idx);
};

#endif // PATHFINDING_H
