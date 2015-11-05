/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
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
    void registro();
    void setArgumentos(QMap<QString, QString> value);
    bool startProcess(QString pathprogram, bool wait, QStringList arguments);
    QMap<QString, QString> validaParametros(QStringList argv, int argc, bool *ok);

private:
    void configurar(const QMap<QString, QString> &param);
    void forcaColeta();
    void install();
    void parametrosIncorretos();
    void uninstall();
    void updateService();

    LogCacic *logcacic;
    QMap<QString, QString> argumentos;
    CacicComm *oCacicComm;
    CACIC_Computer oCacicComputer;
    QString applicationDirPath;
    QString cacicMainFolder;
signals:
    void finished();

public slots:
    void run(QStringList argv, int argc);
};

#endif // INSTALLCACIC_H
