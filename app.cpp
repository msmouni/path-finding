#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QWidget(parent), ui(new Ui::App)
{
    ui->setupUi(this);

    this->setWindowTitle("Pathfinding Visualization");

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
    connect(m_map, SIGNAL(findPath()), this, SLOT(findPath()));
    connect(m_path_finder, SIGNAL(pathFindingRes(RunResult)), this, SLOT(setPathFindingResult(RunResult)));
    connect(ui->platformer_check, SIGNAL(stateChanged(int)), this, SLOT(setPlatformer(int)));

    // Platformer
    ui->platformer_check->setEnabled(true);

    // Files
    connect(ui->loadButton, SIGNAL(pressed()), this, SLOT(loadMap()));
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(saveMap()));

    // Log
    m_log_timer = new QTimer;
    connect(m_log_timer, &QTimer::timeout, this, &App::updateLog);
    m_log_timer->start(M_LOG_TIMER_PERIOD_MS);
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
    ui->platformer_check->setEnabled(true);
    m_path_finding_res.insert(res.m_algo, res.m_path_finding);
}

void App::setPlatformer(int state)
{
    m_path_finding_res.clear();
    m_path_finder->setPlatformer(state == Qt::Checked);
}

void App::findPath()
{
    if (m_map->isReady() && m_path_finder->isReady())
    {
        ui->platformer_check->setEnabled(false);
        m_path_finder->restart();
    }
}

void App::saveMap()
{
    m_files_handler.saveMap(m_map);
}

void App::loadMap()
{
    m_files_handler.loadMap(m_map);
    // Block the signals temporarily: In order to not emit SIGNAL(valueChanged(int))
    ui->nb_rows->blockSignals(true);
    ui->nb_col->blockSignals(true);

    ui->nb_rows->setValue(m_map->getNbRows());
    ui->nb_col->setValue(m_map->getNbColumns());

    ui->nb_rows->blockSignals(false);
    ui->nb_col->blockSignals(false);

    ui->selectAlgo->setCurrentIndex(static_cast<int>(PathFindingAlgos::None));
}

void App::reset()
{
    m_path_finding_res.clear();
    m_path_finder->reinit();
    ui->platformer_check->setEnabled(true);
}

void App::updateLog()
{
    if (!m_map->isReady())
    {
        QString log;

        if (!m_map->isStartSet())
        {
            log += QString("%1 Press 'S' + click on a tile to set the starting tile\n").arg(QChar(0x2022));
        }

        if (!m_map->isTargetSet())
        {
            log += QString("%1 Press 'T' + click on a tile to set the target tile\n").arg(QChar(0x2022));
        }

        setLogText(log);
    }
    else
    {
        setLogText(getPathFindingLog());
    }
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
