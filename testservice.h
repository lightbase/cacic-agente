#ifndef TESTSERVICE_H
#define TESTSERVICE_H

#include <QTest>

class TestService : public QTest
{
    Q_OBJECT
public:
    explicit TestService(QObject *parent = 0);

signals:

public slots:

};

#endif // TESTSERVICE_H
