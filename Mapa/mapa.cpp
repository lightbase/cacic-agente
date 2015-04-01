#include "mapa.h"
#include "ui_mapa.h"

Mapa::Mapa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mapa)
{
    //this->setWindowFlags(this->windowFlags() & Qt::BypassWindowManagerHint);
    this->setWindowFlags(this->windowFlags() & Qt::WindowStaysOnTopHint);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
    this->setWindowState(this->windowState() | Qt::WindowFullScreen);

    ui->setupUi(this);
}

Mapa::~Mapa()
{
    delete ui;
}

void Mapa::on_okButton_clicked()
{
    this->close();
}
