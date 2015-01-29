#ifndef GERCOLS_H
#define GERCOLS_H
#include <QtCore>
#include <QJsonObject>
#include <ccoleta.h>
#include <ccacic.h>
#include "QLogger.h"

class Gercols : public QObject
{
    Q_OBJECT

private:
    CCacic oCacic;
    CColeta *oColeta;
    QJsonObject configReq;
    QLogger::QLoggerManager *logManager;
    bool percorreColeta(const QJsonValue &primeiroValor, const QJsonValue &segundoValor, QJsonValue &jsonRetorno);
    bool verificaColeta(const QJsonObject &coletaAntiga, const QJsonObject &novaColeta);

public:
    explicit Gercols(QObject *parent = 0);

public slots:
    void run();

signals:
    void finished();
    void iniciaConfiguracao();
    void iniciaColeta();
    void fimColeta();
};

#endif // GERCOLS_H
