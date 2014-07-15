#ifndef INSTALLCACIC_H
#define INSTALLCACIC_H

#include <QObject>

class InstallCacic : public QObject
{
    Q_OBJECT
public:
    explicit InstallCacic(QObject *parent = 0);

private:

signals:
    void finished(int);

public slots:
    void run();
};

#endif // INSTALLCACIC_H
