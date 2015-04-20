/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <QtCore>
#include <QString>
#include <QStringList>
#include <QSysInfo>
#include "ccacic.h"
#include "console.h"
#ifdef Q_OS_WIN
    #include "wmi.h"
#endif

class OperatingSystem
{
private:
    int idOs;
    int coletaIdOs();
    QString nomeOs;
    QString coletaNomeOs();
public:
    OperatingSystem();
    QString coletaVersaoOsEmString();
    QJsonObject toJsonObject();
    enum OsList {
        MAC,
        LINUX_DEBIAN,
        LINUX_UBUNTU,
        LINUX_ARCH
    };
    static const enum OsList OsList;

    QString getNomeOs();
    int getIdOs() const;
};

#endif // OPERATINGSYSTEM_H
