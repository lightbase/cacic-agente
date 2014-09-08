#ifndef CACICD_H
#define CACICD_H
#include <QTextStream>
#include <QVariant>
#include <QCryptographicHash>
#include <QCoreApplication>
#include "qtservice.h"
#include "cacictimer.h"

class cacicD : public QtService<QCoreApplication>
{
public:
    QCoreApplication *app;
    CCacic *ccacic;
    CACIC_Computer OCacic_Computer;
    CacicComm *OCacicComm;
    CacicTimer *OCacicTimer;
    cacicD(int argc, char **argv);
    ~cacicD();
    void start();
    void pause();
    void resume();
    void stop();

private:
    bool compararHashMD5(QJsonDocument getconfigfile,QJsonDocument getConfig);
    void registraInicio();
    void registraFim();

public slots:
    bool getTest();
    bool getConfig();

};
#endif // CACICD_H
