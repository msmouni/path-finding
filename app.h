#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QMap>
#include <QTimer>
#include "map.h"
#include "run.h"
#include "files.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class App;
}
QT_END_NAMESPACE

class App : public QWidget
{
    Q_OBJECT

public:
    App(QWidget *parent = nullptr);
    ~App();
private slots:
    void setVisualizationDelay(int val);
    void setPathFindingResult(RunResult);
    void setPlatformer(int);
    void findPath();
    void saveMap();
    void loadMap();
    void reset();
    void updateLog();

private:
    Ui::App *ui;

    const int M_LOG_TIMER_PERIOD_MS = 100;

    Map *m_map;
    PathFindingRunner *m_path_finder;
    FilesHandler m_files_handler;
    // QMap: the items are always sorted by key
    QMap<PathFindingAlgos, PathFindingResult> m_path_finding_res;
    QTimer *m_log_timer;

    void setLogText(QString txt);
    QString getPathFindingLog();
};
#endif // APP_H
