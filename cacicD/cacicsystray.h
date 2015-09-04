#ifndef CACICSYSTRAY_H
#define CACICSYSTRAY_H

#include <QObject>
#include <QMenu>
#include <QSystemTrayIcon>
#include "logcacic.h"
#include "identificadores.h"

class CacicSysTray: public QObject
{
    Q_OBJECT
public:
    CacicSysTray();
    CacicSysTray(QString dirpath);
    void iniciarSysTray();

public slots:
    void sysTrayClicked(QSystemTrayIcon::ActivationReason reason);
    void actionClicked();

private:
    LogCacic *logcacic;
    QString mainFolder;
    QSystemTrayIcon *sysTray;
};

#endif // CACICSYSTRAY_H
