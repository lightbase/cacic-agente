#ifndef GERCOLS_H
#define GERCOLS_H
#include <QtCore>
#include <ccoleta.h>
#include <ccacic.h>

class Gercols : public QObject
{
    Q_OBJECT
private:
    CCacic oCacic;
    CColeta oColeta;
public:
    explicit Gercols(QObject *parent = 0);

public slots:
    void run();

signals:
    void finished();
    void fimColeta();
};

#endif // GERCOLS_H
