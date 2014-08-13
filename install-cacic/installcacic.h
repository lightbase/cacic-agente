#ifndef INSTALLCACIC_H
#define INSTALLCACIC_H

#include <QObject>
#include <QtCore>
#include <cacic_comm.h>
#include <cacic_computer.h>
#include <ccacic.h>

class InstallCacic : public QObject
{
    Q_OBJECT
public:
    explicit InstallCacic(QObject *parent = 0);

    QMap<QString, QString> getArgumentos();
    void setArgumentos(QMap<QString, QString> value);
    QMap<QString, QString> validaParametros(QStringList argv, int argc, bool *ok);

private:
    QMap<QString, QString> argumentos;
    CacicComm oCacicComm;
    CACIC_Computer oCacicComputer;
    CCacic oCacic;
signals:
    void finished();

public slots:
    void run(QStringList argv, int argc);
};

#endif // INSTALLCACIC_H
