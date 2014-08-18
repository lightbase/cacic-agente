#include "gercols.h"

Gercols::Gercols(QObject *parent)
{
    /*Aqui vamos conectar o slot principal. Acho que algo do tipo:
     * sinal iniciaColeta() chama o slot configuraColetas() que verifica
     * quais coletas serao necessárias e realiza a conexão dos sinais com
     * slots de coletas (coletaHardware, coletaSoftware, etc..)
     */
}

void Gercols::run()
{
    QJsonObject coleta;
    /* Pega configurações do Json de configurações localizado
     * na pasta principal do cacic (deverá ser pega do registro,
     * estou tentando implementar isso no installcacic).
     */

    //Inicializa as classes e seta valores necessários oCacic.setCacicMainFolder(), por exemplo.

    //emite sinal para começar a coleta

    //aguarda todas as coletas serem realizadas

    //salva coleta em json
    coleta["computador"] = oColeta.getOComputer().toJsonObject();
    coleta["software"] = QJsonValue::fromVariant(QString("Aqui vai coleta de software"));
    coleta["hardware"] = QJsonValue::fromVariant(QString("Aqui vai coleta de hardware"));
    qDebug() << coleta;
    //salva json em arquivo
    oCacic.setJsonToFile(coleta, "coleta.json");
    //emite sinal "finished" pra finalizar a aplicação
    emit finished();
}
