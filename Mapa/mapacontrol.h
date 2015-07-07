#ifndef MAPACONTROL_H
#define MAPACONTROL_H

#include <QObject>
#include <QtCore>
#include <QMainWindow>
#include <QWidget>
#include "mapa.h"

class MapaControl : public QObject
{
    Q_OBJECT
public:
    explicit MapaControl(QObject *parent = 0);
    ~MapaControl();
    void run(int argc, char *argv[]);

signals:
    void finished();
public slots:

private:
    bool args2Map(int argc, char *argv[], QMap<QString, QString> &map);
    bool getMapa(const QString &server);

    CacicComm *oCacicComm;
    QMainWindow *interface;
    QMutex mutex;
    QWaitCondition wait4finish;
    QString mainFolder;
};

#endif // MAPACONTROL_H
