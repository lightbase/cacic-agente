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
#include "cacicthread.h"
#include "checkmodules.h"
#include "identificadores.h"
#include <servicecontroller.h>

class CacicTimer : public QObject
{
    Q_OBJECT
public:
    CacicTimer(QString dirpath);
    ~CacicTimer();
    QTimer *timer;
    CCacic *ccacic;
    QMutex *cMutex;
    void iniciarTimer();
    QJsonObject getTest(CacicComm &OCacicComm);
    QJsonObject getConfig(CacicComm &OCacicComm);
    void setApplicationDirPath(const QString &value);
    QString getApplicationDirPath();
    void setDirProgram(const QString &value);
    void setPeriodicidadeExecucao(int value);
    bool comunicarGerente();
    int getPeriodicidadeExecucaoAnterior() const;
    void setPeriodicidadeExecucaoAnterior(int value);

signals:
    void finalizar();

private:
    bool verificarEIniciarQMutex();
    bool verificarModulos();
    void reiniciarTimer();
    void iniciarInstancias();
    bool verificarPeriodicidade();
    void lerArquivoConfig( const QJsonObject &jsonConfig);
    void definirDirModulo(QString appDirPath, QString nome);
    int getPeriodicidadeExecucao() const;
    bool removeArquivosEstrangeiros(const QDir &diretorio);
    bool removeCacicAnterior();
    bool verificarseModuloJaFoiExecutado(QString nome, QString hash);
    QString getDirProgram() const;

    QLogger::QLoggerManager *logManager;
    CacicThread *cacicthread;
    QString dirProgram;
    QString applicationDirPath;
    QJsonObject jsonConfig;

    int periodicidadeExecucao = 0;
    int periodicidadeExecucaoAnterior;
    static const int periodicidadeExecucaoPadrao = 240; // Tempo default de execução em minutos.


private slots:
    void mslot();

public slots:
    void iniciarThread();
};

#endif // CACICTIMER_H
