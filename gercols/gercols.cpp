#include "gercols.h"

Gercols::Gercols(QObject *parent)
{
    /*Aqui vamos conectar o slot principal. Acho que algo do tipo:
     * sinal iniciaColeta() chama o slot configuraColetas() que verifica
     * quais coletas serao necessárias e realiza a conexão dos sinais com
     * slots de coletas (coletaHardware, coletaSoftware, etc..)
     */

    /* Cria um json de configuração para teste.
     ******************************************/
    QJsonObject configTeste;
    QJsonObject configComputer;
    configComputer["operating_system"] = QJsonValue::fromVariant(QString(""));
    configComputer["user"] = QJsonValue::fromVariant(QString(""));
    configComputer["network_interface"] = QJsonValue::fromVariant(QString(""));
    configTeste["computer"] = configComputer;
    configTeste["hardware"] = QJsonValue::fromVariant(QString(""));
    configTeste["software"] = QJsonValue::fromVariant(QString(""));

    oCacic.setJsonToFile(configTeste,"configReq.json");
    //Pega chave do registro, que será pega na instalação.
    oCacic.setChaveCrypt(oCacic.getValueFromRegistry("Lightbase", "Cacic", "key").toString());

qDebug() << "TESTE";
    oColeta = new CColeta();

    QObject::connect(this, SIGNAL(iniciaConfiguracao()), oColeta, SLOT(configuraColetas()));
    QObject::connect(this, SIGNAL(iniciaColeta()), oColeta, SLOT(run()));

}

void Gercols::run()
{

    /* Pega configurações do Json de configurações localizado
     * na pasta principal do cacic (deverá ser pega do registro,
     * estou tentando implementar isso no installcacic).
     */

    //Inicializa as classes e seta valores necessários oCacic.setCacicMainFolder(), por exemplo.
    // ... ainda a ser pensado

    emit iniciaConfiguracao();
    //emite sinal para começar a coleta
    emit iniciaColeta();

    oCacic.deleteFile("configReq.json");

    //salva json em arquivo
    if (!oColeta->toJsonObject().isEmpty())
        oCacic.setJsonToFile(oColeta->toJsonObject(), "coleta.json");
    else
        qDebug() << "Erro ao realizar coleta.";

    //O processo de criptografia e IV será tratado depois. Vamos começar a ajudar o Eli, depois vemos esse lance.
//    oCacic.deleteFile("coleta.json");

    //IV sendo gerado como uma string de 32 caracteres
//    std::string strIv = oCacic.genRandomString();

//    QString coletaEncriptada = oCacic.enCrypt(coletaString.toStdString(), strIv);

//    QJsonObject gercolsJson;
//    gercolsJson["coleta"] = QJsonValue::fromVariant(coletaEncriptada);
//    gercolsJson["iv"] = QJsonValue::fromVariant( QString::fromStdString(strIv) );
//    oCacic.setJsonToFile(gercolsJson, "gercols.json");

    //emite sinal "finished" pra finalizar a aplicação
    emit finished();
}
