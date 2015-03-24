#ifndef IDENTIFICADORES_H
#define IDENTIFICADORES_H

#include <QString>
#include <QDir>

#define ROTA_GETTEST "/ws/neo/getTest"
#define ROTA_GETLOGIN "/ws/neo/getLogin"
#define ROTA_GETCONFIG "/ws/neo/config"
#define ROTA_AUTORIZA "/ws/neo/autoriza"
#define ROTA_CONFIRMA "/ws/neo/confirma"
#define ROTA_COLETA "/ws/neo/coleta"
#define ROTA_COLETA_DIFF "/ws/neo/modifications"
#define ROTA_LOG "/ws/neo/logs"
#define LOG_DAEMON "Cacic Daemon"
#define LOG_CACICDEPLOY "Cacic Deploy"
#define LOG_DAEMON_THREAD "Cacic Daemon (Thread)"
#define LOG_DAEMON_TIMER "Cacic Daemon (Timer)"
#define LOG_CACIC_COMM "Cacic Comm"
#define LOG_GERCOLS "Gercols"
#define LOG_GERCOLS_HARDWARE "Gercols (hardware)"
#define LOG_INSTALL_CACIC "Install Cacic"
#define LOG_SOCKET_LISTENER "Cacic Daemon (Socket Listener)"
#define LOG_CHECKMODULES "CheckModules"
#define LOG_CACIC_MESSAGE "Cacic Message"
#define CACIC_SERVICE_NAME "cacicdaemon"

class Identificadores
{
public:
    Identificadores();

    static const QString ENDERECO_PATCH_CACIC;
    static const QString AGENTE_VERSAO;

    enum {
        INSTALAR = 0,
        DESINSTALAR = 1,
        MODIFICAR = 2
    };

};

#endif // IDENTIFICADORES_H
