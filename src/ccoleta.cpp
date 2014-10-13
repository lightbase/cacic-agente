#include "ccoleta.h"

CColeta::CColeta(QObject *parent)
{

}

void CColeta::coletaHardware()
{
    qDebug() << "coletaHardware() começando sua execução";
    oHardware.iniciaColeta();
    qDebug() << "coletaHardware() executado";
    emit hardwareFinish();
    emit beginSoftware();
}


void CColeta::coletaSoftware()
{
    qDebug() << "coletaSoftware() começando sua execução";
    oSoftware.iniciaColeta();
    qDebug() << "coletaSoftware() executado";
    emit softwareFinish();
}

void CColeta::configuraColetas(){
    QObject::connect(this, SIGNAL(beginHardware()), this, SLOT(coletaHardware()));
    QObject::connect(this, SIGNAL(beginSoftware()), this, SLOT(coletaSoftware()));
    QObject::connect(this, SIGNAL(softwareFinish()), this, SLOT(softwareReady()));
    QObject::connect(this, SIGNAL(hardwareFinish()), this, SLOT(hardwareReady()));
    emit beginHardware();
}

bool CColeta::waitToCollect()
{
    int timeout = 0;
    while(!(this->softwareIsFinish && this->hardwareIsFinish) && timeout != 30){
        QThread::sleep(1);
        timeout++;
    }
    return true;
}

void CColeta::run()
{
    /* Versão do json de testes */
    QJsonObject coleta = oCacic.getJsonFromFile("getConfig.json");
//    qDebug() << coleta;
    QJsonObject actions = coleta["reply"].toObject()["agentcomputer"].toObject()["actions"].toObject();
//    qDebug() << actions["col_hard"] << actions["col_soft"];
    if( actions.contains("col_hard") && actions["col_hard"].toBool()){
        this->hardwareIsFinish = false;
        emit beginHardware();
    } else
        this->hardwareIsFinish = true;

    if ( actions.contains("col_soft") && actions["col_soft"].toBool()){
        this->softwareIsFinish = false;
        emit beginSoftware();
    } else
        this->softwareIsFinish = true;

}

QJsonObject CColeta::toJsonObject()
{
    QJsonObject coletaJson;
    if (this->hardwareIsFinish && this->softwareIsFinish){
        coletaJson["computer"] = oComputer.toJsonObject();
        coletaJson["software"] = oSoftware.toJsonObject();
        coletaJson["hardware"] = oHardware.toJsonObject();
    }
    return coletaJson;
}

void CColeta::hardwareReady()
{
    this->hardwareIsFinish = true;
}

void CColeta::softwareReady()
{
    this->softwareIsFinish = true;
}

CACIC_Computer CColeta::getOComputer() const
{
    return oComputer;
}

cacic_software CColeta::getOSoftware() const
{
    return oSoftware;
}

cacic_hardware CColeta::getOHardware() const
{
    return oHardware;
}
