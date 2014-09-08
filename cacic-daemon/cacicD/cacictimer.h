#ifndef CACICTIMER_H
#define CACICTIMER_H
#include <QtCore>
#include <QTimer>
#include <QDebug>
#include "ccacic.h"
#include "cacic_comm.h"
#include "cacic_computer.h"

class CacicTimer : public QObject
{
    Q_OBJECT
public:
    CacicTimer();
    QTimer *timer;
    void iniciarTimer(int x);

signals:
    void getTestSi();
    void getConfigSi();

private slots:
    void mslot();
};

#endif // CACICTIMER_H
