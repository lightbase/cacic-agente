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
#include  "cacicthread.h"

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
    void iniciarTimer();
    bool getTest();
    bool getConfig();
    bool Md5IsEqual(QVariant document01,QVariant document02);
    void setApplicationDirPath(const QString &value);
    QString getApplicationDirPath();
    void setDirProgram(const QString &value);
    void setPeriodicidadeExecucao(int value);

private:
    bool verificarEIniciarQMutex();
    QStringList verificarModulos();
    void reiniciarTimer();
    QLogger::QLoggerManager *logManager;
    CacicThread *cacicthread;
    QString dirProgram;
    QString applicationDirPath;
    QString getDirProgram() const;
    void iniciarInstancias();
    void verificarPeriodicidadeJson();
    void lerArquivoConfig( const QJsonObject &jsonConfig);
    void definirDirModulo(QString appDirPath, QString nome);
    QJsonObject jsonConfig;
    int periodicidadeExecucao = 14400; // 4*3600. Tempo default de execução.
    int getPeriodicidadeExecucao() const;

private slots:
    void mslot();
};

#endif // CACICTIMER_H
