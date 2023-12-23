#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QWidget(parent), ui(new Ui::App)
{
    ui->setupUi(this);

    m_map = ui->graphicsView->getMapPtr();

    for (int i=0; i<=NB_ALGOS; i++){
        ui->selectAlgo->addItem(getAlgoName(i));
    }


    ui->nb_rows->setValue(m_map->getNbRows());
    ui->nb_col->setValue(m_map->getNbColumns());

    connect(ui->nb_rows, SIGNAL(valueChanged(int)), m_map, SLOT(setNbRows(int)));
    connect(ui->nb_col, SIGNAL(valueChanged(int)), m_map, SLOT(setNbColumns(int)));

    connect(ui->delaySlider, SIGNAL(valueChanged(int)), this, SLOT(setVisualizationDelay(int)));

    m_path_finding = new PathFindingRunner(m_map);
    connect(m_map, SIGNAL(updated()), m_path_finding, SLOT(reinit()));
    connect(ui->selectAlgo, SIGNAL(activated(int)), m_path_finding, SLOT(setAlgo(int)));

    connect(m_map, SIGNAL(stopPathFinding()), m_path_finding, SLOT(terminate()));
    connect(m_map, SIGNAL(findPath()), m_path_finding, SLOT(restart()));
}

App::~App()
{
    delete ui;
}

void App::setVisualizationDelay(int val)
{
    ui->delay->setText(QString::number(val));
    m_path_finding->setVisualDelayMs(val);
}
