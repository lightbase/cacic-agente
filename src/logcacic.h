#ifndef LOGCACIC_H
#define LOGCACIC_H

#include "QLogger.h"
#include "cacic_comm.h"
#include "identificadores.h"
#include "ccacic.h"

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
    bool realizarEnvioDeLogs(const QStringList &logLvls);

private:
    QString identificador;
    QString enderecoArquivoLog;
    QLogger::LogLevel resolverLevel(CacicLogLevel level);

};

#endif // LOGCACIC_H
