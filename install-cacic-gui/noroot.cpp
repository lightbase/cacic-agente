#include "noroot.h"
#include "ui_noroot.h"

NoRoot::NoRoot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NoRoot)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/usr/share/cacic/cacic-log.png"));
}

NoRoot::~NoRoot()
{
    delete ui;
}

void NoRoot::on_btFechar_clicked()
{
    emit finished();
}
