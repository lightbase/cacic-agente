/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef CHECKMODULES_H
#define CHECKMODULES_H

#include <ccacic.h>
#include <cacic_comm.h>
#include <cacic_computer.h>
#include "logcacic.h"
#include <cmodulearray.h>

class CheckModules
{
public:
    CheckModules(const QString &workingPath, const QString &workingModule);
    ~CheckModules();
    //    QStringList verificaModulos();
    bool start();
    CModuleArray getModules() const;
private:

    QString applicationUrl;
    QString cacicMainFolder;
    CModuleArray modules;
    bool verificaModulo(const QString &moduloName, const QString &moduloHash, const QString &moduloUrl);
    //    void lerArquivoConfig(const QJsonObject& jsonConfig);
    LogCacic *logcacic;
};

#endif // CHECKMODULES_H
