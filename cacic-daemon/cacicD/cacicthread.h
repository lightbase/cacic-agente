#ifndef CACICTHREAD_H
#define CACICTHREAD_H
#include <QtCore>
#include <QMutex>
#include "QLogger.h"
#include "ccacic.h"
#include "cacic_comm.h"
#include "identificadores.h"

class CacicThread : public QThread
{
public:
    CacicThread(QString applicationDirPath);
    void run();
    void setModuloDirPath(const QString &value);
    void setCMutex(QMutex *value);
    void setNomeModulo(const QString &value);
    void setCcacic(CCacic *value);

private:
    void iniciarInstancias();
    void registraInicioColeta();
    void registraFimColeta(bool tipo);
    bool verificaForcarColeta();
    bool realizarEnviodeColeta();
    bool enviarColetaDiff();
    bool enviarColeta();
    void registrarDataEnvioDeColeta();
    void iniciarModulo();

    QString moduloDirPath;
    QString applicationDirPath;
    QLogger::QLoggerManager *logManager;
    QMutex *cMutex;
    QString nomeModulo;
    CCacic *ccacic;
};

#endif // CACICTHREAD_H
