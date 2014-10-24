#include "gercols.h"

Gercols::Gercols(QObject *parent)
{
    oColeta = new CColeta;
    //Pega chave do registro, que será pega na instalação.
    oCacic.setCacicMainFolder(oCacic.getValueFromRegistry("Lightbase", "Cacic", "key").toString());
    oCacic.setChaveCrypt(oCacic.getValueFromRegistry("Lightbase", "Cacic", "key").toString());
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(oCacic.getCacicMainFolder() + "/Logs/cacic.log","Gercols",QLogger::InfoLevel);
    logManager->addDestination(oCacic.getCacicMainFolder() + "/Logs/cacic.log","Gercols",QLogger::ErrorLevel);

    QObject::connect(this, SIGNAL(iniciaConfiguracao()), oColeta, SLOT(configuraColetas()));
    QObject::connect(this, SIGNAL(iniciaColeta()), oColeta, SLOT(run()));

}

void Gercols::run()
{

    emit iniciaConfiguracao();
    //emite sinal para começar a coleta
    emit iniciaColeta();

    oColeta->waitToCollect();
    //salva json em arquivo
    if (!oColeta->toJsonObject().isEmpty()){
        QLogger::QLog_Info("Gercols", QString("Coleta realizada com sucesso."));
        oCacic.setJsonToFile(oColeta->toJsonObject(), "coleta.json");
    } else {
        QLogger::QLog_Error("Gercols", QString("Falha ao realizar coleta."));
    }
    emit finished();
}
