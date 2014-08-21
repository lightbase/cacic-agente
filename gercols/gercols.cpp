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
    configComputer["operating_system"] = "";
    configComputer["user"] = "";
    configComputer["network_interface"] = "";
    configTeste["computer"] = configComputer;
    configTeste["hardware"] = "";
    configTeste["software"] = "";

    oCacic.setJsonToFile(configTeste,"configReq.json");
    /******************************************/

qDebug() << "TESTE";
    oColeta = new CColeta();

    QObject::connect(this, SIGNAL(iniciaConfiguracao()), oColeta, SLOT(configuraColetas()));
    QObject::connect(this, SIGNAL(iniciaColeta()), oColeta, SLOT(run()));

}

void Gercols::run()
{
    QJsonObject coleta;
    /* Pega configurações do Json de configurações localizado
     * na pasta principal do cacic (deverá ser pega do registro,
     * estou tentando implementar isso no installcacic).
     */

    //Inicializa as classes e seta valores necessários oCacic.setCacicMainFolder(), por exemplo.
    // ... ainda a ser pensado

    emit iniciaConfiguracao();
    //emite sinal para começar a coleta
    emit iniciaColeta();

    //salva json em arquivo
    oCacic.setJsonToFile(oColeta->toJsonObject(), "coleta.json");
    oCacic.deleteFile("configReq.json");
    //emite sinal "finished" pra finalizar a aplicação
    emit finished();
}
