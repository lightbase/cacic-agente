#ifndef MAPACONTROL_H
#define MAPACONTROL_H

#include <QObject>
#include <QtCore>
#include <QMainWindow>
#include <QWidget>
#include "mapa.h"
#include "cacic_comm.h"

class MapaControl : public QObject
{
    Q_OBJECT
public:
    explicit MapaControl(QObject *parent = 0);
    ~MapaControl();

signals:
    void finished();
public slots:
    void run(QStringList args);

private:
    bool args2Map(QStringList args, QMap<QString, QString> &map);
    bool getMapa();
    bool getMapaP();

    QMainWindow *interface;
    QString mainFolder;
};

#endif // MAPACONTROL_H
