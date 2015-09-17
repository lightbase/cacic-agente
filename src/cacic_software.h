/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/

#ifndef CACIC_SOFTWARE_H
#define CACIC_SOFTWARE_H
#include <QtCore>
#include <ccacic.h>
#include <console.h>
#include <operatingsystem.h>
#ifdef Q_OS_WIN
    #include <vqtconvert.h>
    #include <vregistry.h>
#endif

class cacic_software
{
public:
    cacic_software();
    void iniciaColeta();
    QJsonObject toJsonObject();

    QHash<QString, QStringList> exceptions() const;
    void setExceptionClasses(const QHash<QString, QStringList> &exceptions);

private:
#ifdef Q_OS_WIN
    QJsonObject coletaWin();
#else
    QJsonObject coletaLinux();
    QJsonObject coletaArch();
    QJsonObject coletaDebian();
#endif

    QJsonObject coletaSoftware;
    QHash<QString, QStringList> _exceptions;
};

#endif // CACIC_SOFTWARE_H
