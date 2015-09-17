#ifndef CACICSYSTRAY_H
#define CACICSYSTRAY_H

#include <QMainWindow>
#include <QtCore>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMenu>
#include <QSystemTrayIcon>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class CacicSysTray: public QWidget
{
    Q_OBJECT
public:
    explicit CacicSysTray(const QString &dirpath, QWidget *parent =0);
    ~CacicSysTray();
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
