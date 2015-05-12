/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
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
#include "logcacic.h"
#include "cacicthread.h"
#include "checkmodules.h"
#include "identificadores.h"
#include <servicecontroller.h>
#ifdef Q_OS_WIN
    #include "vqtconvert.h"
    #include "vregistry.h"
#endif

class CacicTimer : public QObject
{
    Q_OBJECT
public:
    CacicTimer(QString dirpath);
    ~CacicTimer();
    QTimer *timer;
    QMutex *cMutex;
    void iniciarTimer();
    QJsonObject getTest(CacicComm &OCacicComm);
    QJsonObject getConfig(CacicComm &OCacicComm);
    void setApplicationDirPath(const QString &value);
    QString getApplicationDirPath();
    void setDirProgram(const QString &value);
    bool comunicarGerente();
    void setPeriodicidadeExecucaoAnterior(int value);
    int getPeriodicidadeExecucaoAnterior() const;

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
    void setPeriodicidadeExecucao(int value);
    int getPeriodicidadeExecucao() const;
    bool removeArquivosEstrangeiros(const QDir &diretorio);
    bool removeCacicAnterior();
    bool verificarseModuloJaFoiExecutado(QString nome, QString hash);
    bool verificaForcarColeta();
    bool realizarEnviodeColeta();
    bool realizarEnvioDeLogs(const QStringList &logLvls);
    bool enviarColetaDiff();
    bool enviarColeta();
    bool enviarLogs();
    void registrarDataEnvioDeColeta();
    QString getDirProgram() const;
#ifdef Q_OS_WIN
    void changeCacicVersion();
#endif

    CacicThread *cacicthread;
    QString cacicMainFolder;
    QString dirProgram;
    QString applicationDirPath;
    QJsonObject jsonConfig;
    LogCacic *logcacic;
    int periodicidadeExecucao = 0;
    int periodicidadeExecucaoAnterior = 0;
    static const int periodicidadeExecucaoPadrao = 240; // Tempo default de execução em minutos.

private slots:
    void mslot();

public slots:
    void iniciarThread();
};

#endif // CACICTIMER_H
