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
    void iniciarTimer();
    bool getTest();
    bool getConfig();
    bool Md5IsEqual(QVariant document01,QVariant document02);
    void setApplicationDirPath(const QString &value);
    QString getApplicationDirPath();
    void setDirProgram(const QString &value);
    void setPeriodicidadeExecucao(int value);

private:
    void registraFimColeta(QString msg);
    void registraInicioColeta();
    QStringList verificarModulos();
    void reiniciarTimer();
    QLogger::QLoggerManager *logManager;
    QString dirProgram;
    QString applicationDirPath;
    QString getDirProgram() const;
    void iniciarModulo(QString modulo);
    void iniciarInstancias();
    void verificarPeriodicidadeJson();
    void lerArquivoConfig( const QJsonObject &jsonConfig);
    void definirDirModulo(QString appDirPath, QString nome);
    QJsonObject jsonConfig;
    int periodicidadeExecucao;
    int getPeriodicidadeExecucao() const;
    QList<QMap<QString,QString> > metodosDownload;
    QMap<QString, QString> moduleMap; // key = hash md5, value = nome do modulo

private slots:
    void mslot();
};

#endif // CACICTIMER_H
