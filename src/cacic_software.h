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

private:
#ifdef Q_OS_WIN
    QJsonObject coletaWin();
#else
    QJsonObject coletaLinux();
    QJsonObject coletaArch();
    QJsonObject coletaDebian();
#endif

    QJsonObject coletaSoftware;
};

#endif // CACIC_SOFTWARE_H
