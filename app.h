#ifndef APP_H
#define APP_H

#include <QWidget>
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

private:
    Ui::App *ui;

    Map *m_map;
    PathFindingRunner *m_path_finding;
};
#endif // APP_H
