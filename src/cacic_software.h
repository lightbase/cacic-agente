#ifndef CACIC_SOFTWARE_H
#define CACIC_SOFTWARE_H
#include <QtCore>
#include <ccacic.h>
#include <console.h>
#include <operatingsystem.h>
#include <vqtconvert.h>
#include <vregistry.h>

class cacic_software
{
public:
    cacic_software();
    void iniciaColeta();
    QJsonObject toJsonObject();

private:
    QJsonObject coletaWin();
    QJsonObject coletaLinux();
    QJsonObject coletaArch();
    QJsonObject coletaDebian();

    CCacic oCacic;
    QJsonObject coletaSoftware;
};

#endif // CACIC_SOFTWARE_H
