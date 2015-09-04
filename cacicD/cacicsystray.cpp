#include "cacicsystray.h"

CacicSysTray::CacicSysTray()
{
    iniciarSysTray();
}

CacicSysTray::CacicSysTray(QString dirpath)
{
    if(!dirpath.isEmpty()) {
        this->mainFolder = dirpath;
        logcacic = new LogCacic(LOG_SYSTRAY, mainFolder+"/Logs");
        logcacic->escrever(LogCacic::InfoLevel,QStrin("SystemTray constructor."));
    }
    iniciarSysTray();
}

void CacicSysTray::iniciarSysTray()
{
    if(QSystemTrayIcon::isSystemTrayAvailable()) {
        if(logcacic != NULL)
            logcacic->escrever(LogCacic::InfoLevel,QString("Inicializando SystemTray"));

        sysTray = new QSystemTrayIcon(this);

        sysTray->setIcon(QIcon(":/cacic-logo.png"));
        sysTray->setToolTip("Cacic");

        QMenu *menu;
        QAction* action;

        menu = new QMenu();
        menu->addAction("generica");
        menu->addSeparator();
        action = new QAction(QString("alvo"),this);
        connect(action,&QAction::triggered,this,&CacicSysTray::actionClicked);
        menu->addAction(action);
        QMenu *newMenu = menu->addMenu("submenu");

        sysTray->setContextMenu(menu);

        sysTray->show();

        connect(sysTray,&QSystemTrayIcon::activated,this,&CacicSysTray::sysTrayClicked);

    } else {
        if(logcacic != NULL)
            logcacic->escrever(LogCacic::InfoLevel,QString("Falha ao iniciar SystemTray"));
    }
}

void CacicSysTray::sysTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "sysTrayClicked" << endl;
    QMenu* menu = sysTray->contextMenu();

    QAction* actionPressed = menu->exec(sysTray->geometry().topLeft());
    actionPressed->activate(QAction::Trigger);
}

void CacicSysTray::actionClicked()
{
    if(logcacic != NULL)
        logcacic->escrever(LogCacic::InfoLevel,QString("Systray action clicked."));
}
