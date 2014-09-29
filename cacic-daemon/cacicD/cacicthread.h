#ifndef CACICTHREAD_H
#define CACICTHREAD_H
#include <QtCore>
#include <QMutex>
#include "QLogger.h"

class CacicThread : public QThread
{
public:
    CacicThread(QString applicationDirPath);
    void run();
    void setModuloDirPath(const QString &value);
    void setCMutex(QMutex *value);

private:
    QString moduloDirPath;
    QString applicationDirPath;
    QLogger::QLoggerManager *logManager;
    void iniciarInstancias();
    void registraInicioColeta();
    void registraFimColeta(QString msg);
    void iniciarModulo();
    QMutex *cMutex;
};

#endif // CACICTHREAD_H
