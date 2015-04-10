#ifndef CACICTHREAD_H
#define CACICTHREAD_H
#include <QtCore>
#include <QMutex>
#include "ccacic.h"
#include "logcacic.h"
#include "identificadores.h"

class CacicThread : public QThread
{

private:
    Q_OBJECT
    CacicThread();
    void registraInicioExecucao();
    void registraExecucao(bool tipo, QString status);
    void registrarDataEnvioDeColeta();
    void iniciarModulo();
    void setLastStatus(const QProcess::ExitStatus &value);
    void setLastError(const QString &value);
    LogCacic *logcacic;
    QString moduloDirPath;
    QString applicationDirPath;
    QMutex *cMutex;
    QString nomeModulo;
    int timeoutSec;
    QProcess::ExitStatus lastStatus;
    QString lastError;

public:
    CacicThread(QString applicationDirPath);
    void run();
    void setModuloDirPath(const QString &value);
    void setCMutex(QMutex *value);
    void setNomeModulo(const QString &value);

    int getTimeoutSec() const;
    void setTimeoutSec(int value);
    QProcess::ExitStatus getLastStatus() const;
    QString getLastError() const;

signals:
    void endExecution();
};

#endif // CACICTHREAD_H
