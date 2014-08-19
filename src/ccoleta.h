#ifndef CCOLETA_H
#define CCOLETA_H
#include <QtCore>
#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <ccacic.h>
#include <cacic_computer.h>
#include <cacic_software.h>


class CColeta : public QObject
{
    Q_OBJECT

private:
    CCacic oCacic;
    CACIC_Computer oComputer;
    cacic_software oSoftware;

public:
    explicit CColeta(QObject *parent = 0);

    CACIC_Computer getOComputer() const;
    cacic_software getOSoftware() const;

    QJsonObject toJsonObject();

public slots:
    void coletaHardware();
    void coletaSoftware();
    void configuraColetas();
    void run();

signals:
    void beginComputer();
    void beginHardware();
    void beginNetworkInterfaces();
    void beginOperatingSystem();
    void beginSoftware();
    void beginUser();
    void finished();
};

#endif // CCOLETA_H
