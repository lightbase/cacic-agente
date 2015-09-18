#ifndef CACICSYSTRAY_H
#define CACICSYSTRAY_H

#include <QMainWindow>
#include <QtCore>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class CacicSysTray: public QWidget
{
    Q_OBJECT
public:
    explicit CacicSysTray(const QString &dirpath, QTcpSocket * const socket, QWidget *parent =0);
    ~CacicSysTray();
    void iniciarSysTray();

    QMenu* setupMenu();

public slots:
    void sysTrayClicked(QSystemTrayIcon::ActivationReason reason);
    void infosClicked();
    void forcarClicked();
    void finalizarClicked();

private:
    LogCacic *logcacic;
    QString mainFolder;
    QSystemTrayIcon *sysTray;
    QTcpSocket *socket;
};

#endif // CACICSYSTRAY_H
