#ifndef IDENTIFICADORES_H
#define IDENTIFICADORES_H

#include <QString>

class Identificadores
{
public:
    Identificadores();
    static const QString ROTA_GETTEST = "/ws/neo/getTest";
    static const QString ROTA_GETCONFIG = "/ws/neo/config";
    static const QString ROTA_COLETA = "/ws/neo/coleta";
    static const QString LOG_DAEMON = "Cacic Daemon";
    static const QString LOG_DAEMON_THREAD = "Cacic Daemon (Thread)";
    static const QString LOG_DAEMON_TIMER = "Cacic Daemon (Timer)";

};

#endif // IDENTIFICADORES_H
