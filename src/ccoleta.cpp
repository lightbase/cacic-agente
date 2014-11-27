#include "ccoleta.h"

CColeta::CColeta(QObject *parent)
{

}

void CColeta::coletaHardware()
{
    oHardware.iniciaColeta();
    emit hardwareFinish();
}


void CColeta::coletaSoftware()
{
    oSoftware.iniciaColeta();
    emit softwareFinish();
}

/********************************************************
 * Conecta os sinais com os slots corretos, ou seja,
 * sempre que um sinal for emitido, o slot será executado
 ********************************************************/
void CColeta::configuraColetas(){
    QObject::connect(this, SIGNAL(beginHardware()), this, SLOT(coletaHardware()));
    QObject::connect(this, SIGNAL(beginSoftware()), this, SLOT(coletaSoftware()));
    QObject::connect(this, SIGNAL(softwareFinish()), this, SLOT(softwareReady()));
    QObject::connect(this, SIGNAL(hardwareFinish()), this, SLOT(hardwareReady()));
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
    QJsonObject coleta = oCacic.getJsonFromFile("getConfig.json");
    QJsonObject actions = coleta["agentcomputer"].toObject()["actions"].toObject();

    //se for pra coletar hardware...
    if( actions.contains("col_hard") && actions["col_hard"].toBool()){
        QLogger::QLog_Info("Gercols", QString("Iniciando coleta de hardware."));
        this->hardwareIsFinish = false;
        emit beginHardware();
    } else
        this->hardwareIsFinish = true;

    //se for pra coletar software...
    if ( actions.contains("col_soft") && actions["col_soft"].toBool()){
        QLogger::QLog_Info("Gercols", QString("Iniciando coleta de software."));
        this->softwareIsFinish = false;
        emit beginSoftware();
    } else
        this->softwareIsFinish = true;

}

/*********************************************
 * Transforma todos os dados da classe em json
 *********************************************/
QJsonObject CColeta::toJsonObject()
{
    QJsonObject coletaJson;
    coletaJson["computador"] = oComputer.toJsonObject();
    QJsonObject hardware = oHardware.toJsonObject();
    hardware["NetworkAdapterConfiguration"] = oComputer.toJsonObject()["networkDevices"].toArray();
    coletaJson["hardware"] = hardware;
    coletaJson["software"] = oSoftware.toJsonObject();
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
