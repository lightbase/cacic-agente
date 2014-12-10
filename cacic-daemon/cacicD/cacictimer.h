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
#include "checkmodules.h"
#include "identificadores.h"

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
    QJsonObject getTest();
    QJsonObject getConfig();
    void setApplicationDirPath(const QString &value);
    QString getApplicationDirPath();
    void setDirProgram(const QString &value);
    void setPeriodicidadeExecucao(int value);
    bool comunicarGerente();

signals:
    void finalizar();

private:
    bool verificarEIniciarQMutex();
    bool verificarModulos();
    void reiniciarTimer();
    QString getDirProgram() const;
    void iniciarInstancias();
    bool verificarPeriodicidade();
    void lerArquivoConfig( const QJsonObject &jsonConfig);
    void definirDirModulo(QString appDirPath, QString nome);
    int getPeriodicidadeExecucao() const;
    void iniciarThread();

    QLogger::QLoggerManager *logManager;
    CacicThread *cacicthread;
    CheckModules *checkModules;
    QString dirProgram;
    QString applicationDirPath;
    QJsonObject jsonConfig;
    int periodicidadeExecucao;
    const int periodicidadeExecucaoPadrao = 240; // Tempo default de execução em minutos.

private slots:
    void mslot();
};

#endif // CACICTIMER_H
