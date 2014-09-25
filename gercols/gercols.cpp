#include "gercols.h"

Gercols::Gercols(QObject *parent)
{

    /* Cria um json de configuração para teste.
     *************************************************************************/
    QJsonObject configTeste;
    QJsonObject configComputer;
    configComputer["operating_system"] = QJsonValue::fromVariant(QString(""));
    configComputer["user"] = QJsonValue::fromVariant(QString(""));
    configComputer["network_interface"] = QJsonValue::fromVariant(QString(""));
    configTeste["computer"] = configComputer;
    configTeste["hardware"] = QJsonValue::fromVariant(QString(""));
    configTeste["software"] = QJsonValue::fromVariant(QString(""));

    oCacic.setJsonToFile(configTeste,"configReq.json");
    /*************************************************************************/

    oColeta = new CColeta;
    //Pega chave do registro, que será pega na instalação.
    oCacic.setChaveCrypt(oCacic.getValueFromRegistry("Lightbase", "Cacic", "key").toString());

    QObject::connect(this, SIGNAL(iniciaConfiguracao()), oColeta, SLOT(configuraColetas()));
    QObject::connect(this, SIGNAL(iniciaColeta()), oColeta, SLOT(run()));

}

void Gercols::run()
{
    qDebug() << "run...";
    emit iniciaConfiguracao();
    //emite sinal para começar a coleta
    emit iniciaColeta();

    /* Json de teste sendo excluído
     */
    oCacic.deleteFile("configReq.json");
    /***********************************/

    oColeta->waitToCollect();
    //salva json em arquivo
    if (!oColeta->toJsonObject().isEmpty())
        oCacic.setJsonToFile(oColeta->toJsonObject(), "coleta.json");
    else
        qDebug() << "Erro ao realizar coleta.";

    //O processo de criptografia e IV será tratado depois.

    //emite sinal "finished" pra finalizar a aplicação
    emit finished();
}
