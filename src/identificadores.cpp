#include "identificadores.h"

Identificadores::Identificadores()
{
}

const QString Identificadores::ROTA_GETLOGIN = QString("/ws/neo/getLogin");

const QString Identificadores::ROTA_GETTEST = QString("/ws/neo/getTest");

const QString Identificadores::ROTA_GETCONFIG = QString("/ws/neo/config");

const QString Identificadores::ROTA_COLETA = QString("/ws/neo/coleta");

const QString Identificadores::ROTA_COLETA_DIFF = QString("/ws/neo/modifications");

const QString Identificadores::ENDERECO_PATCH_CACIC = QString(
            #ifdef Q_OS_LINUX
            QDir::rootPath()+"usr/share/cacic"
            #endif
            #ifdef Q_OS_WIN
            QDir::rootPath()+"cacic"
            #endif
            );

const QString Identificadores::LOG_DAEMON = QString("Cacic Daemon");

const QString Identificadores::LOG_DAEMON_THREAD = QString("Cacic Daemon (Thread)");

const QString Identificadores::LOG_DAEMON_TIMER = QString("Cacic Daemon (Timer)");

const QString Identificadores::LOG_CACIC_COMM = QString("Cacic Comm");

const QString Identificadores::LOG_GERCOLS = QString("Gercols");

const QString Identificadores::LOG_INSTALL_CACIC = QString("Install Cacic");

const QString Identificadores::LOG_SOCKET_LISTENER = QString("Cacic Daemon (Socket Listener)");

const QString Identificadores::LOG_CHECKMODULES = QString("CheckModules");

const QString Identificadores::AGENTE_VERSAO = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD); //Valor vindo do .pro

const QString Identificadores::CACIC_SERVICE_NAME = QString("cacicdaemon");
