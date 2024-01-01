#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QWidget(parent), ui(new Ui::App)
{
    ui->setupUi(this);

    m_map = ui->graphicsView->getMapPtr();

    // Algo Selection
    for (int i = 0; i <= NB_ALGOS; i++)
    {
        ui->selectAlgo->addItem(getAlgoName(i));
    }

    // Map size
    ui->nb_rows->setValue(m_map->getNbRows());
    ui->nb_col->setValue(m_map->getNbColumns());
    connect(ui->nb_rows, SIGNAL(valueChanged(int)), m_map, SLOT(setNbRows(int)));
    connect(ui->nb_col, SIGNAL(valueChanged(int)), m_map, SLOT(setNbColumns(int)));

    // Visualization delay
    connect(ui->delaySlider, SIGNAL(valueChanged(int)), this, SLOT(setVisualizationDelay(int)));

    // Path Finding Runner
    m_path_finder = new PathFindingRunner(m_map);
    connect(m_map, SIGNAL(updated()), this, SLOT(reset()));
    connect(ui->selectAlgo, SIGNAL(activated(int)), m_path_finder, SLOT(setAlgo(int)));
    connect(m_map, SIGNAL(stopPathFinding()), m_path_finder, SLOT(terminate()));
    connect(m_map, SIGNAL(findPath()), m_path_finder, SLOT(restart()));
    connect(m_path_finder, SIGNAL(pathFindingRes(RunResult)), this, SLOT(setPathFindingResult(RunResult)));
}

App::~App()
{
    delete ui;
}

void App::setVisualizationDelay(int val)
{
    ui->delay->setText(QString::number(val));
    m_path_finder->setVisualDelayMs(val);
}

void App::setPathFindingResult(RunResult res)
{
    m_path_finding_res.insert(res.m_algo, res.m_path_finding);

    setLogText(getPathFindingLog());
}

void App::reset()
{
    m_path_finding_res.clear();
    m_path_finder->reinit();
    setLogText(getPathFindingLog());
}

void App::setLogText(QString txt)
{
    ui->logText->setText(txt);
}

QString App::getPathFindingLog()
{
    QString log_txt;
    for (int i = 1; i <= NB_ALGOS; i++)
    {
        log_txt += getAlgoName(i) + ":\n" + m_path_finding_res.value(getAlgoFromInt(i)).toText() + "\n\n";
    }

    return log_txt;
}
