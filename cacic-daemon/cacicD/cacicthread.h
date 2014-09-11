#ifndef CACICTHREAD_H
#define CACICTHREAD_H

#include <QThread>

class CacicThread : public QThread
{
    Q_OBJECT
public:
    explicit CacicThread(QObject *parent = 0);

signals:

public slots:

};

#endif // CACICTHREAD_H
