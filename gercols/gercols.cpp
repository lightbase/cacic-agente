#include "gercols.h"

Gercols::Gercols(QObject *parent) :
    QObject(parent)
{

}

void Gercols::run(QStringList argv, int argc)
{
    std::cout << "Gercols" << std::endl;
    readConfig();
    std::cout << "Configuration file read." << std::endl;
}

void Gercols::readConfig()
{
    CCacic OCacic;
    QString file = "configRequest.json";

    this->configJson = OCacic.getJsonFromFile(file);
}

QJsonObject Gercols::getConfigJson()
{
    return configJson;
}
