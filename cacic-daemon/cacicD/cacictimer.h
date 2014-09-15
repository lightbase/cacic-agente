#ifndef CACICTIMER_H
#define CACICTIMER_H
#include <QtCore>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QMutex>
#include <QProcess>
#include <QMap>
#include "ccacic.h"
#include "cacic_comm.h"
#include "cacic_computer.h"
#include "QLogger.h"

class CacicTimer : public QObject
{
    Q_OBJECT
public:
    CacicTimer(QString dirpath);
    ~CacicTimer();
    QTimer *timer;
    CacicComm *OCacicComm;
    CACIC_Computer OCacic_Computer;
    CCacic *ccacic;
    QMutex *cMutex;
    void iniciarTimer(int x);
    bool getTest();
    bool getConfig();
    bool Md5IsEqual(QVariant document01,QVariant document02);
    void setApplicationDirPath(const QString &value);
    void setDirProgram(const QString &value);

private:
    void registraFim(QString msg);
    void registraInicio();
    QLogger::QLoggerManager *logManager;
    QString dirProgram;
    //QProcess *myProcess;
    QString applicationDirPath;
    QString getDirProgram() const;
    void iniciarGercols();
    void iniciarInstancias();
    void definirDirGercols(QString appDirPath);
    QJsonObject jsonConfig;
    QString metodoDownload;
    QMap<QString, QString> moduleMap; // key = hash md5, value = nome do modulo

private slots:
    void mslot();
};

#endif // CACICTIMER_H
