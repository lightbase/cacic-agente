#include "trayicon.h"

TrayIcon::TrayIcon()
{
    Q_INIT_RESOURCE(image);

    this->icone = new QIcon("qrc:/trayIcon/img/cacic_logo.png");
    this->sysTrayIcon = new QSystemTrayIcon(this);
    this->sysTrayIcon->setIcon(*icone);
    this->createActions();
    this->createTrayIcon();
}

void TrayIcon::show()
{
    this->sysTrayIcon->show();
}

void TrayIcon::createActions()
{
    this->quitAction = new QAction("&Quit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void TrayIcon::createTrayIcon()
{
    this->trayIconMenu = new QMenu(this);
    this->trayIconMenu->addAction(quitAction);

    this->sysTrayIcon = new QSystemTrayIcon(this);
    this->sysTrayIcon->setContextMenu(trayIconMenu);
}
