#include "identificadores.h"

Identificadores::Identificadores()
{
}

const QString Identificadores::AGENTE_VERSAO = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
const QString Identificadores::ENDERECO_PATCH_CACIC = QString(
            #ifdef Q_OS_LINUX
            QDir::rootPath()+"usr/share/cacic"
            #endif
            #ifdef Q_OS_WIN
            QDir::rootPath()+"cacic"
            #endif
            );

