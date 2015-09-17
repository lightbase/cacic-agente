#include "ccoleta.h"

CColeta::CColeta(QObject *parent)
{
    logcacic = new LogCacic(LOG_GERCOLS, Identificadores::ENDERECO_PATCH_CACIC+"/Logs");
}

void CColeta::coletaHardware()
{
    oHardware.iniciaColeta();
    logcacic->escrever(LogCacic::InfoLevel, "Coleta de hardware finalizada.");
    emit hardwareFinish();
}


void CColeta::coletaSoftware()
{
    oSoftware.iniciaColeta();
    logcacic->escrever(LogCacic::InfoLevel, QString("Coleta de software finalizada."));
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
    QJsonObject coleta = CCacic::getJsonFromFile("getConfig.json");
    QJsonObject actions = coleta["agentcomputer"].toObject()["actions"].toObject();

    //se for pra coletar hardware...
    if( actions.contains("col_hard") && actions["col_hard"].toBool()){
        this->setHardwareExceptionClasses(coleta["agentcomputer"].toObject()["exceptionClasses"].toObject()["hardware"].toObject());
        logcacic->escrever(LogCacic::InfoLevel, QString("Iniciando coleta de hardware."));
        this->hardwareIsFinish = false;
        emit beginHardware();
    } else
        this->hardwareIsFinish = true;

    //se for pra coletar software...
    if ( actions.contains("col_soft") && actions["col_soft"].toBool()){
        this->setSoftwareExceptionClasses(coleta["agentcomputer"].toObject()["exceptionClasses"].toObject()["software"].toObject());
        logcacic->escrever(LogCacic::InfoLevel, QString("Iniciando coleta de software."));
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
    QJsonObject hardware = oHardware.toJsonObject();
    hardware["NetworkAdapterConfiguration"] = oComputer.toJsonObject()["networkDevices"].toArray();
    coletaJson["computador"] = oComputer.toJsonObject();
    coletaJson["hardware"] = hardware;
    coletaJson["software"] = oSoftware.toJsonObject();
    return coletaJson;
}

void CColeta::setHardwareExceptionClasses(const QHash<QString, QStringList> &exceptions)
{
    this->oHardware.setExceptionClasses(exceptions);
}

void CColeta::setHardwareExceptionClasses(const QJsonObject &exceptions)
{
    if(!exceptions.isEmpty()){
        QHash<QString, QStringList> hard;
        for (QJsonObject::const_iterator i = exceptions.constBegin(); i != exceptions.constEnd(); i++){
            QStringList tmp;
            foreach(QJsonValue value, i.value().toArray()){
                tmp.append(value.toString());
            }
            hard[i.key()] = tmp;
        }
        this->oHardware.setExceptionClasses(hard);
    }
}

void CColeta::setSoftwareExceptionClasses(const QHash<QString, QStringList> &exceptions)
{
    this->oSoftware.setExceptionClasses(exceptions);
}

void CColeta::setSoftwareExceptionClasses(const QJsonObject &exceptions)
{
    if(!exceptions.isEmpty()){
        QHash<QString, QStringList> soft;
        for (QJsonObject::const_iterator i = exceptions.constBegin(); i != exceptions.constEnd(); i++){
            QStringList tmp;
            foreach(QJsonValue value, i.value().toArray()){
                tmp.append(value.toString());
            }
            soft[i.key()] = tmp;
        }
        this->oSoftware.setExceptionClasses(soft);
    }
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
