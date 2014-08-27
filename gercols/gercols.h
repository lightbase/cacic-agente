#ifndef GERCOLS_H
#define GERCOLS_H
#include <QtCore>
#include <QJsonObject>
#include <ccoleta.h>
#include <ccacic.h>
#include <cacic_comm.h>

class Gercols : public QObject
{
    Q_OBJECT
private:
    CacicComm oCacicComm;
    CCacic oCacic;
    CColeta *oColeta;
    QJsonObject configReq;

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
