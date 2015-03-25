#ifndef CACICTHREAD_H
#define CACICTHREAD_H
#include <QtCore>
#include <QMutex>
#include "logcacic.h"
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


private:
    void registraInicioColeta();
    void registraFimColeta(bool tipo);
    void registrarDataEnvioDeColeta();
    void iniciarModulo();
    LogCacic *logcacic;
    QString moduloDirPath;
    QString applicationDirPath;
    QMutex *cMutex;
    QString nomeModulo;

};

#endif // CACICTHREAD_H
