#include "uninstallcacic.h"
#include "ui_uninstallcacic.h"

UninstallCacic::UninstallCacic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UninstallCacic)
{
    ui->setupUi(this);
}

UninstallCacic::~UninstallCacic()
{
    delete ui;
}

void UninstallCacic::on_pushButton_clicked()
{
    emit finished();
}

void UninstallCacic::on_pushButton_2_clicked()
{
    if(!ui->desinstalar->isChecked() && !ui->modificar->isChecked()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Atenção");
        msgBox.setText("Escolha uma das opções.");
        msgBox.exec();
    }
    if(ui->desinstalar->isChecked()){
        getObjInstallCacic()->setGui(true);
        getObjInstallCacic()->setModoDeExecucao(Identificadores::DESISNSTALAR);
        getObjInstallCacic()->setWindowTitle("Desinstalar o Cacic");
        getObjInstallCacic()->show();
        this->close();
    }
    if(ui->modificar->isChecked()){
        getObjInstallCacic()->setGui(true);
        getObjInstallCacic()->setModoDeExecucao(Identificadores::MODIFICAR);
        getObjInstallCacic()->show();
        this->close();
    }
}
InstallCacicGui *UninstallCacic::getObjInstallCacic() const
{
    return objInstallCacic;
}

void UninstallCacic::setObjInstallCacic(InstallCacicGui *value)
{
    objInstallCacic = value;
}

