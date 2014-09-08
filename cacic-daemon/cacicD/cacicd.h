#ifndef CACICD_H
#define CACICD_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QVariant>
#include "qtservice.h"
#include "cacic_comm.h"
#include "cacic_computer.h"


class cacicD : public QtService<QCoreApplication>
{
public:
    cacicD(int argc, char **argv);
    ~cacicD();
    void start();
    void pause();
    void resume();
    void stop();
    QCoreApplication *app;
    void saveJson(QJsonDocument document, QString fileName);
    QJsonDocument loadJson(QString fileName);

private:
    QString testPath;
    QString testIniPath;
    QJsonObject session;
    CACIC_Computer OCacic_Computer;
    bool getTest();
    bool getConfig();
};

#endif // CACICD_H
