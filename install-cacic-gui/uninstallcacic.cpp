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
        QMessageBox(QMessageBox::Information,
                    "Erro",
                    "Escolha uma das opções disponíveis.",
                    QMessageBox::Ok).exec();
    }
    if(ui->desinstalar->isChecked()){
        getObjInstallCacic()->setGui(true);
        getObjInstallCacic()->setModoDeExecucao(Identificadores::DESINSTALAR);
        getObjInstallCacic()->setWindowTitle("Desinstalar o Cacic");
        getObjInstallCacic()->show();
        //this->close();
    }
    if(ui->modificar->isChecked()){
        getObjInstallCacic()->setGui(true);
        getObjInstallCacic()->setModoDeExecucao(Identificadores::MODIFICAR);
        getObjInstallCacic()->show();
        //this->close();
    }
}
CCacic *UninstallCacic::getOCCacic() const
{
    return oCCacic;
}

void UninstallCacic::setOCCacic(CCacic *value)
{
    oCCacic = value;
}

InstallCacicGui *UninstallCacic::getObjInstallCacic() const
{
    return objInstallCacic;
}

void UninstallCacic::setObjInstallCacic(InstallCacicGui *value)
{
    objInstallCacic = value;
}

