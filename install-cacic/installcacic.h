#ifndef INSTALLCACIC_H
#define INSTALLCACIC_H

#include <QObject>
#include <QtCore>
#include <cacic_comm.h>
#include <cacic_computer.h>
#include <ccacic.h>
#include <stdlib.h>
#include "iostream"
#include "QLogger.h"

class InstallCacic : public QObject
{
    Q_OBJECT
public:
    explicit InstallCacic(QObject *parent = 0);
    ~InstallCacic();

    QMap<QString, QString> getArgumentos();
    void setArgumentos(QMap<QString, QString> value);
    QMap<QString, QString> validaParametros(QStringList argv, int argc, bool *ok);
    bool startProcess(QString pathprogram, bool wait, QStringList arguments);
    void registro();

private:
    void uninstall();
    QMap<QString, QString> argumentos;
    CacicComm *oCacicComm;
    CACIC_Computer oCacicComputer;
    CCacic oCacic;
    QLogger::QLoggerManager *logManager;
    QString applicationDirPath;
    void parametrosIncorretos();
    void updateService();
    void configurar(const QMap<QString, QString> &param);
    void install();
signals:
    void finished();

public slots:
    void run(QStringList argv, int argc);
};

#endif // INSTALLCACIC_H
