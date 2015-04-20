/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef LOGCACIC_H
#define LOGCACIC_H

#include "QLogger.h"

class LogCacic
{

public:
    LogCacic(QString identificadorLOG, QString enderecoArquivoLog);
    ~LogCacic();

    enum CacicLogLevel {
        TraceLevel = 0,
        DebugLevel = 1,
        InfoLevel = 2,
        WarnLevel = 3,
        ErrorLevel = 4,
        FatalLevel = 5,
        MessageControler = 6
    };

    void escrever(CacicLogLevel level, QString msg);
    QString getLevelEmString(CacicLogLevel level);
    QString resolverEnderecoArquivo(CacicLogLevel level);
    CacicLogLevel levelName2Value(const QString &levelName) throw(int);

private:
    QString identificador;
    QString enderecoArquivoLog;
    QLogger::LogLevel resolverLevel(CacicLogLevel level);

};

#endif // LOGCACIC_H
