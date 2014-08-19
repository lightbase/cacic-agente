#ifndef CACIC_SOFTWARE_H
#define CACIC_SOFTWARE_H
#include <QtCore>
#include <ccacic.h>
class cacic_software
{
public:
    cacic_software();
    void iniciaColeta();
    QJsonObject toJsonObject();

private:
    QJsonObject coletaWin();
    void coletaLinux();

    CCacic oCacic;
    QJsonObject coletaSoftware;
};

#endif // CACIC_SOFTWARE_H
