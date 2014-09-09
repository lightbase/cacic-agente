#ifndef CACICTIMER_H
#define CACICTIMER_H
#include <QtCore>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include "ccacic.h"
#include "cacic_comm.h"
#include "cacic_computer.h"
#include "QLogger.h"

class CacicTimer : public QObject
{
    Q_OBJECT
public:
    CacicTimer();
    QTimer *timer;
    CacicComm *OCacicComm;
    CACIC_Computer OCacic_Computer;
    CCacic *ccacic;
    //QLogger::QLoggerManager *manager;
    void iniciarTimer(int x, QString applicationDirPath);
    bool getTest();
    bool getConfig();
    bool compararHashMD5(QJsonDocument getconfigfile,QJsonDocument getConfig);
    QString getApplicationDirPath() const;
    void setApplicationDirPath(const QString &value);

private:
    void registraFim();
    void registraInicio();
    QString applicationDirPath;

private slots:
    void mslot();
};

#endif // CACICTIMER_H
