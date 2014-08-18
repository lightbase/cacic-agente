#ifndef GERCOLS_H
#define GERCOLS_H

#include <QtCore>
#include <QObject>
#include <QJsonObject>
#include <QString>
#include <iostream>
#include <../src/ccacic.h>

class Gercols : public QObject
{
    Q_OBJECT
public:
    explicit Gercols(QObject *parent = 0);

    void readConfig();
    QJsonObject getConfigJson();

private:
    QJsonObject configJson;

signals:
    void finished();

public slots:
    void run(QStringList argv, int argc);

};

#endif // GERCOLS_H
