#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);
}

App::~App()
{
    delete ui;
}

