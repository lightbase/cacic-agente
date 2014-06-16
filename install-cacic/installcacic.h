#ifndef INSTALLCACIC_H
#define INSTALLCACIC_H

#include <QObject>
#include <QCoreApplication>
#include <iostream>
#include <QStringList>
#include <QTimer>

using namespace std;

class InstallCacic : public QObject
{
    Q_OBJECT
public:
    // variaveis

    explicit InstallCacic(QObject *parent = 0);

    QCoreApplication *getApp() const;
    void setApp(const QCoreApplication &value);

private:
    QCoreApplication app;
signals:

public slots:
    void runInstall(QCoreApplication*);

};

#endif // INSTALLCACIC_H
