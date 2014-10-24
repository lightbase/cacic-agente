#ifndef CHECKMODULES_H
#define CHECKMODULES_H

#include <ccacic.h>
#include <cacic_comm.h>
#include <cacic_computer.h>
#include "QLogger.h"

class CheckModules
{
public:
    CheckModules(const QString &workingPath);
    ~CheckModules();
//    QStringList verificaModulos();
    bool start();
    QVariantMap getModules() const;
private:
    CCacic oCacic;
    CacicComm oCacicComm;
    QString applicationUrl;
    QVariantMap modules; //modules["name"] = hash;
    bool verificaModulo(const QString &moduloName, const QString &moduloHash);
//    void lerArquivoConfig(const QJsonObject& jsonConfig);

    QLogger::QLoggerManager *logManager;
};

#endif // CHECKMODULES_H
