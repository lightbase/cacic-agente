#ifndef CHECKMODULES_H
#define CHECKMODULES_H

#include <ccacic.h>
#include <cacic_comm.h>
#include <cacic_computer.h>

class CheckModules
{
public:
    CheckModules(const QString &workingPath);
//    QStringList verificaModulos();
    bool start();
private:
    CCacic oCacic;
    QVariantMap modules; //modules["name"] = hash;
//    void lerArquivoConfig(const QJsonObject& jsonConfig);
};

#endif // CHECKMODULES_H
