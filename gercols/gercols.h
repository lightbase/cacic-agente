#ifndef GERCOLS_H
#define GERCOLS_H
#include <QtCore>
#include <QJsonObject>
#include <ccoleta.h>
#include <ccacic.h>
#include "logcacic.h"
#include "identificadores.h"

class Gercols : public QObject
{
    Q_OBJECT

public:
    explicit Gercols(QObject *parent = 0);

public slots:
    void run();

signals:
    void finished();
    void iniciaConfiguracao();
    void iniciaColeta();
    void fimColeta();

private:

    bool percorreColeta(const QJsonValue &primeiroValor, const QJsonValue &segundoValor, QJsonValue &jsonRetorno);
    bool verificaColeta(const QJsonObject &coletaAntiga, const QJsonObject &novaColeta);

    LogCacic *logcacic;
    CColeta *oColeta;
    QJsonObject configReq;
    QString cacicMainFolder;

};

#endif // GERCOLS_H
