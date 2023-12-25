#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QMap>
#include "map.h"
#include "run.h"

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
    void reset();

private:
    Ui::App *ui;

    Map *m_map;
    PathFindingRunner *m_path_finder;
    // QMap: the items are always sorted by key
    QMap<PathFindingAlgos, PathFindingResult> m_path_finding_res;

    void setLogText(QString txt);
};
#endif // APP_H
