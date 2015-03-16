#ifndef LOGCACIC_H
#define LOGCACIC_H

#include "QLogger.h"
#include "ccacic.h"

class LogCacic
{

public:
    LogCacic(QString identificadorLOG, QString enderecoArquivoLog);
    ~LogCacic();
    enum CacicLogLevel {
        TraceLevel = 0,
        DebugLevel,
        InfoLevel,
        WarnLevel,
        ErrorLevel,
        FatalLevel,
        MessageControler
    };
    void escrever(CacicLogLevel level, QString msg);
    QString getLevelEmString(LogCacic::CacicLogLevel level);
    QString resolverEnderecoArquivo(LogCacic::CacicLogLevel level);

private:
    QString identificador;
    QString enderecoArquivoLog;
    QLogger::LogLevel resolverLevel(LogCacic::CacicLogLevel level);

};

#endif // LOGCACIC_H