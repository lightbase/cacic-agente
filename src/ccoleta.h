#ifndef CCOLETA_H
#define CCOLETA_H
#include <QtCore>
#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <cacic_computer.h>


class CColeta : public QObject
{
    Q_OBJECT

private:
    QJsonObject coleta;

    CACIC_Computer oComputer;

public:
    explicit CColeta(QObject *parent = 0);

    QJsonObject getColeta() const;
    CACIC_Computer getOComputer() const;
    void setColeta(QJsonObject config);

public slots:
    void coletaComputer();
    void coletaHardware();
    void coletaNetworkInterfaces();
    void coletaOperatingSystem();
    void coletaSoftware();
    void coletaUser();
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
