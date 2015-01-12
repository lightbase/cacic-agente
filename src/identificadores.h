#ifndef IDENTIFICADORES_H
#define IDENTIFICADORES_H

#include <QString>

class Identificadores
{
public:
    Identificadores();

    static const QString ROTA_GETLOGIN;

    static const QString ROTA_GETTEST;

    static const QString ROTA_GETCONFIG;

    static const QString ROTA_COLETA;

    static const QString ROTA_COLETA_DIFF;

    static const QString ENDERECO_PATCH_CACIC;

    static const QString LOG_DAEMON;

    static const QString LOG_DAEMON_THREAD;

    static const QString LOG_DAEMON_TIMER;

    static const QString LOG_CACIC_COMM;

    static const QString LOG_INSTALL_CACIC;

    static const QString LOG_GERCOLS;

    static const QString AGENTE_VERSAO;

    static const QString CACIC_SERVICE_NAME;

    enum {
        INSTALAR = 0,
        DESISNSTALAR = 1,
        MODIFICAR = 2
    };

};

#endif // IDENTIFICADORES_H
