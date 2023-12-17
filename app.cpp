#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QWidget(parent), ui(new Ui::App)
{
    ui->setupUi(this);

    m_map = ui->graphicsView->getMapPtr();

    m_path_finding = new PathFindingRunner(m_map);

    connect(m_map, SIGNAL(stopPathFinding()), m_path_finding, SLOT(terminate()));
    connect(m_map, SIGNAL(findPath()), m_path_finding, SLOT(restart()));
}

App::~App()
{
    delete ui;
}
