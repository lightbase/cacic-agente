#include "ccoleta.h"

CColeta::CColeta(QObject *parent)
{

}

void CColeta::coletaComputer()
{
    qDebug() << "coletaComputer() executado";
    QJsonObject coletaComputer = coleta["computer"].toObject();
    if( coletaComputer.contains("operating_system") )
        emit beginOperatingSystem();
    if( coletaComputer.contains("user") )
        emit beginUser();

}

void CColeta::coletaHardware()
{
    qDebug() << "coletaHardware() executado";
    QJsonObject coletaHardware = coleta["hardware"].toObject();
}

void CColeta::coletaNetworkInterfaces()
{
    qDebug() << "coletaOperatingSystem() executado";

//    QJsonObject coletaComputer = coleta["computer"].toObject();
//    coletaComputer["network_interfaces"] = QJsonValue::fromVariant(QString(oComputer.getNetworkInterface()));

//    coleta["computer"] = coletaComputer;
}

void CColeta::coletaOperatingSystem()
{
    qDebug() << "coletaOperatingSystem() executado";

    QJsonObject coletaComputer = coleta["computer"].toObject();
    coletaComputer["operating_system"] = QJsonValue::fromVariant(QString(oComputer.getOs().getNomeOs()));

    coleta["computer"] = coletaComputer;
}

void CColeta::coletaSoftware()
{
    qDebug() << "coletaSoftware() executado";
    QJsonObject coletaSoftware = coleta["software"].toObject();
}

void CColeta::coletaUser()
{
    qDebug() << "coletauser() executado";

    QJsonObject coletaComputer = coleta["computer"].toObject();
    coletaComputer["user"] = QJsonValue::fromVariant(QString::fromStdString(oComputer.getUser()));

    coleta["computer"] = coletaComputer;
}

void CColeta::configuraColetas(){
    QObject::connect(this, SIGNAL(beginComputer()), this, SLOT(coletaComputer()));
    QObject::connect(this, SIGNAL(beginNetworkInterfaces()), this, SLOT(coletaNetworkInterfaces()));
    QObject::connect(this, SIGNAL(beginOperatingSystem()), this, SLOT(coletaOperatingSystem()));
    QObject::connect(this, SIGNAL(beginUser()), this, SLOT(coletaUser()));
    QObject::connect(this, SIGNAL(beginHardware()), this, SLOT(coletaHardware()));
    QObject::connect(this, SIGNAL(beginSoftware()), this, SLOT(coletaSoftware()));
}

QJsonObject CColeta::getColeta() const
{
    return coleta;
}

CACIC_Computer CColeta::getOComputer() const
{
    return oComputer;
}

void CColeta::run()
{
    if( coleta.contains("computer") )
        emit beginComputer();
    if( coleta.contains("hardware") )
        emit beginHardware();
    if ( coleta.contains("software") )
        emit beginSoftware();
}

void CColeta::setColeta(QJsonObject config)
{
    this->coleta = config;
}
