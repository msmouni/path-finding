#ifndef MAP_H
#define MAP_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMutex>
#include "tile.h"

class Map : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    Map(QSizeF tile_size, int nb_rows, int nb_columns);

    QRectF boundingRect() const override;

    const QPoint &getStartIdx();
    const QPoint &getTargetIdx();
    const TileType &getTileType(const QPoint &indx);
    const TileType &getTileType(const int &idx_x, const int &idx_y);
    void setTileType(const QPoint &idx, TileType tile_type);
    void setTileType(const int &idx_x, const int &idx_y, TileType tile_type);
    const int &getNbRows();
    const int &getNbColumns();
    void clearVisited();
    bool isReady();
    bool isTargetSet();
    bool isStartSet();
    bool containsTile(const int &pos_x, const int &pos_y);
    bool containsTile(const QPoint &pos);
    void loadMap(QVector<QVector<TileType>>);

public slots:
    void setNbRows(int nb_rows);
    void setNbColumns(int nb_columns);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // This event is called when the item (or its scene or view) loses focus.
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void updated();
    void findPath();
    void stopPathFinding();

private:
    QSizeF m_tile_size;
    int m_nb_rows;
    int m_nb_columns;
    Qt::MouseButton m_mouse_boutton;
    bool m_mouse_button_clicked;
    QPoint m_last_mouse_indexes;
    bool m_s_key_pressed;
    bool m_t_key_pressed;

    QVector<QVector<Tile *>> m_tiles;

    QPoint m_start_idx;
    bool m_start_set;
    QPoint m_target_idx;
    bool m_target_set;

    // The mutex is locked when QMutexLocker is created. If locked, the mutex will be unlocked when the QMutexLocker is destroyed.
    QMutex m_mutex;

    void init(QVector<QVector<TileType>> = {});
    void setStartIdx(QPoint idx);
    void setTargetIdx(QPoint idx);

    QPoint getIndexesFromPointF(QPointF point);
    void tileClicked(QPoint tile_indexes);

    void tryToFindPath();
};

#endif // MAP_H
