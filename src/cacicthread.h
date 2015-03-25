#ifndef CACICTHREAD_H
#define CACICTHREAD_H
#include <QtCore>
#include <QMutex>
#include "logcacic.h"
#include "identificadores.h"

class CacicThread : public QThread
{
public:
    CacicThread(QString applicationDirPath);
    void run();
    void setModuloDirPath(const QString &value);
    void setCMutex(QMutex *value);
    void setNomeModulo(const QString &value);


    int getTimeout() const;
    void setTimeout(int value);

private:
    void registraInicioExecucao();
    void registraExecucao(bool tipo, QString status);
    void registrarDataEnvioDeColeta();
    void iniciarModulo();
    LogCacic *logcacic;
    QString moduloDirPath;
    QString applicationDirPath;
    QMutex *cMutex;
    QString nomeModulo;
    int timeout;

};

#endif // CACICTHREAD_H
