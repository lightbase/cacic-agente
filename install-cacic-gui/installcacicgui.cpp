#include "installcacicgui.h"
#include "ui_installcacicgui.h"

InstallCacicGui::InstallCacicGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstallCacicGui)
{
    ui->setupUi(this);
    instanciarObjetos();
}

InstallCacicGui::~InstallCacicGui()
{
    delete ui;
}

void InstallCacicGui::on_pushButton_clicked()
{
    if(verificarDadosInseridos()){
        ui->lErrorMsg->setText(QString("Iniciando a instalação ..."));
        QStringList parametros;
        parametros.append(QString("-host=" + ui->leHost->text().toStdString()));
        parametros.append(QString("-user=" + ui->leUsuario->text().toStdString()));
        parametros.append(QString("-pass=" + ui->leSenha->text().toStdString()));
        if(iniciarInstalacao(parametros)){

        }else{

        }
    }else{
        ui->lErrorMsg->setText(QString("Preencha os campos corretamente."));
        ui->lErrorMsg->adjustSize();
        ui->lErrorMsg->setHidden(false);
    }
}

bool InstallCacicGui::verificarDadosInseridos(){
    if(!ui->leHost->text().isEmpty() && !ui->leHost->text().isNull()){
        if(!ui->leUsuario->text().isEmpty() && !ui->leUsuario->text().isNull()){
            if(!ui->leSenha->text().isEmpty() && !ui->leSenha->text().isNull()){
                return true;
            }
        }
    }
    return false;
}

void InstallCacicGui::instanciarObjetos(){
    ui->widget->setWindowTitle(QString("Instalação do Cacic"));
    ui->lErrorMsg->setHidden(true);
    ui->leHost->setPlaceholderText(QString("Endereço do gerente"));
    ui->leUsuario->setPlaceholderText(QString("usuário"));
    ui->leSenha->setPlaceholderText(QString("senha"));
}

bool InstallCacicGui::iniciarInstalacao(QStringList parametros){

}
