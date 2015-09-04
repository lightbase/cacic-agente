/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
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
#define ROTA_LOG "/ws/neo/log"
#define ROTA_MAPA_FORM "/ws/neo/mapa/form"
#define ROTA_MAPA_LDAP "/ws/neo/mapa/ldap"
#define ROTA_MAPA_GETMAPA "/ws/neo/mapa/config"
#define LOG_DAEMON "Cacic Daemon"
#define CHKSYS "Check Cacic"
#define LOG_CACICDEPLOY "Cacic Deploy"
#define LOG_DAEMON_THREAD "Cacic Daemon (Thread)"
#define LOG_DAEMON_TIMER "Cacic Daemon (Timer)"
#define LOG_CACIC_COMM "Cacic Comm"
#define LOG_GERCOLS "Gercols"
#define LOG_GERCOLS_HARDWARE "Gercols (hardware)"
#define LOG_SYSTRAY "System Tray Icon"
#define LOG_INSTALL_CACIC "Install Cacic"
#define LOG_MAPA "Mapa"
#define LOG_SOCKET_LISTENER "Cacic Daemon (Socket Listener)"
#define LOG_CHECKMODULES "CheckModules"
#define LOG_CACIC_MESSAGE "Cacic Message"
#define CACIC_SERVICE_NAME "cacicdaemon"
#define N_LOGS_ENVIO 10

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
