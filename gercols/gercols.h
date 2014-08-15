#ifndef GERCOLS_H
#define GERCOLS_H

#include <QJsonObject>

class Gercols
{
public:
    Gercols();

    void readConfig();
    QJsonObject getConfigJson();
};

#endif // GERCOLS_H
