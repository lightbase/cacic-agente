#ifndef FORCACOLETATIMER_H
#define FORCACOLETATIMER_H
#include <QTimer>
#include <QObject>
#include "QLogger.h"
#include "cacic_comm.h"
#include "ccacic.h"

class ForcaColetaTimer : public QObject
{
    Q_OBJECT
public:
    ForcaColetaTimer(QString dirpath);
    ~ForcaColetaTimer();
    QTimer *timer;
    CacicComm *OCacicComm;
    CCacic *ccacic;
    QMutex *cMutex;
    void iniciarTimer();
    QJsonObject getTest();
    QJsonObject getConfig();
    bool comunicarGerente();
    void setApplicationDirPath(const QString &value);
    void setPeriodicidadeExecucao(int value);

private:
    void reiniciarTimer();
    void iniciarInstancias();
    bool verificarPeriodicidade();
    void lerArquivoConfig( const QJsonObject &jsonConfig);
    void verificarPeriodicidade();
    QLogger::QLoggerManager *logManager;
    QString applicationDirPath;
    const int periodicidadeExecucaoPadrao = 30;
    int periodicidadeExecucao;


signals:
    void finalizar();

public slots:
    mslot();
};

#endif // FORCACOLETATIMER_H
