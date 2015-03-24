#include "logcacic.h"

LogCacic::LogCacic(QString identificadorLOG, QString enderecoArquivoLog)
{
    this->identificador = identificadorLOG;
    this->enderecoArquivoLog = enderecoArquivoLog;
}

LogCacic::~LogCacic()
{
}

void LogCacic::escrever(LogCacic::CacicLogLevel level, QString msg)
{
    QLogger::QLoggerManager::getInstance()->addDestination(resolverEnderecoArquivo(level).toStdString().c_str(),
                                                           this->identificador,
                                                           resolverLevel(level));
    QLogger::QLog_(this->identificador, resolverLevel(level), msg);
    QLogger::QLoggerManager::getInstance()->cleanDestination();
}

QString LogCacic::resolverEnderecoArquivo(LogCacic::CacicLogLevel level){
    switch (level) {
    case LogCacic::TraceLevel:
        return this->enderecoArquivoLog + "/cacic_trace.log";
        break;
    case LogCacic::DebugLevel:
        return this->enderecoArquivoLog + "/cacic_debug.log";
        break;
    case LogCacic::InfoLevel:
        return this->enderecoArquivoLog + "/cacic.log";
        break;
    case LogCacic::WarnLevel:
        return this->enderecoArquivoLog + "/cacic_warn.log";
        break;
    case LogCacic::ErrorLevel:
        return this->enderecoArquivoLog + "/cacic_error.log";
        break;
    case LogCacic::FatalLevel:
        return this->enderecoArquivoLog + "/cacic_fatalerror.log";
        break;
    case LogCacic::MessageControler:
        return this->enderecoArquivoLog + "/cacic_messages.log";
        break;
    }
    return this->enderecoArquivoLog + "/cacic.log";
}

QLogger::LogLevel LogCacic::resolverLevel(LogCacic::CacicLogLevel level){
    switch (level) {
    case LogCacic::TraceLevel:
        return QLogger::TraceLevel;
        break;
    case LogCacic::DebugLevel:
        return QLogger::DebugLevel;
        break;
    case LogCacic::InfoLevel:
        return QLogger::InfoLevel;
        break;
    case LogCacic::WarnLevel:
        return QLogger::WarnLevel;
        break;
    case LogCacic::ErrorLevel:
        return QLogger::ErrorLevel;
        break;
    case LogCacic::FatalLevel:
        return QLogger::FatalLevel;
        break;
    default:
        //para remover warning do compilador.
        break;
    }
    return QLogger::InfoLevel;
}

QString LogCacic::getLevelEmString(LogCacic::CacicLogLevel level){
    switch (level) {
    case LogCacic::TraceLevel:
        return "TraceLevel";
        break;
    case LogCacic::DebugLevel:
        return "DebugLevel";
        break;
    case LogCacic::InfoLevel:
        return "InfoLevel";
        break;
    case LogCacic::WarnLevel:
        return "WarnLevel";
        break;
    case LogCacic::ErrorLevel:
        return "ErrorLevel";
        break;
    case LogCacic::FatalLevel:
        return "FatalLevel";
        break;
    default:
        //para remover warning do compilador.
        break;
    }
    return "InfoLevel";
}

LogCacic::CacicLogLevel LogCacic::levelName2Value(const QString &levelName) throw(int){

    if (levelName == "TraceLevel")
        return LogCacic::TraceLevel;
    else if (levelName == "DebugLevel")
        return LogCacic::DebugLevel;
    else if (levelName == "InfoLevel")
        return LogCacic::InfoLevel;
    else if (levelName == "WarnLevel")
        return LogCacic::WarnLevel;
    else if (levelName == "ErrorLevel")
        return LogCacic::ErrorLevel;
    else if (levelName == "FatalLevel")
        return LogCacic::FatalLevel;
    else
        throw -1;

}

bool LogCacic::realizarEnvioDeLogs(const QStringList &logLvls) {
    bool ok = false;

    if( logLvls.isEmpty() )
        return ok;

    foreach ( QString stringLvl, logLvls ){

        LogCacic::CacicLogLevel level = levelName2Value( stringLvl );

        CCacic *ccacic = new CCacic();
        QJsonObject jsonColeta = ccacic->getJsonFromFile(resolverEnderecoArquivo(level));
        delete ccacic;

        if (!jsonColeta.isEmpty()){

            CacicComm *OCacicComm = new CacicComm();
            OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
            OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
            OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
            QJsonObject retornoColeta;

            escrever(LogCacic::InfoLevel, QString("Enviando Log "+getLevelEmString(level)+" ao gerente."));

            retornoColeta = OCacicComm->comm(ROTA_LOG, &ok, jsonColeta , true);
            if (ok){
                if(!retornoColeta.isEmpty() && !retornoColeta.contains("error")){
                    escrever(LogCacic::InfoLevel, "Log enviado com sucesso.");
                    return true;
                } else if(retornoColeta.contains("error")) {
                    escrever(LogCacic::ErrorLevel, QString("Falha ao enviar log "
                                                                     +getLevelEmString(level)
                                                                     + " para o gerente: " + retornoColeta["error"].toString()));
                    return false;
                }
                return ok;
            } else {
                escrever(LogCacic::ErrorLevel, QString("Falha ao enviar aquivo de log "
                                                                 + getLevelEmString(level)
                                                                 + " para o gerente: Arquivo de Log vazio ou inexistente."));
                return false;
            }
        }
    }
    return ok;
}
