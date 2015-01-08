#ifndef TRAYICON_H
#define TRAYICON_H
#include <QIcon>
#include <QtGui>
#include <qsystemtrayicon.h>
#include <QDialog>
#include <QAction>
#include <QMenu>

class TrayIcon : public QDialog
{
    Q_OBJECT
public:
    TrayIcon();
    void show();
private:
    void createActions();
    void createTrayIcon();
    QIcon *icone;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;
    QSystemTrayIcon *sysTrayIcon;
};

#endif // TRAYICON_H
