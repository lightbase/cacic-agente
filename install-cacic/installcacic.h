#ifndef INSTALLCACIC_H
#define INSTALLCACIC_H

#include <QObject>
#include <QtCore>
#include <QLocalSocket>
#include <cacic_comm.h>
#include <cacic_computer.h>
#include <ccacic.h>
#include <stdlib.h>
#include <servicecontroller.h>
#include "iostream"
#include "logcacic.h"

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
    LogCacic *logcacic;
    void uninstall();
    QMap<QString, QString> argumentos;
    CacicComm *oCacicComm;
    CACIC_Computer oCacicComputer;
    CCacic oCacic;
    QString applicationDirPath;
    void parametrosIncorretos();
    void updateService();
    void configurar(const QMap<QString, QString> &param);
    void install();
    void forcaColeta();

signals:
    void finished();

public slots:
    void run(QStringList argv, int argc);
};

#endif // INSTALLCACIC_H
