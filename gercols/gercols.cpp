#include "gercols.h"

Gercols::Gercols(QObject *parent)
{
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
