#include "cacicsystray.h"

CacicSysTray::CacicSysTray(const QString &dirpath, QWidget *parent) :
    QWidget(parent)
{
    if(!dirpath.isEmpty()) {
        this->mainFolder = dirpath;
        logcacic = new LogCacic(LOG_CACICINTERFACE, mainFolder+"/Logs");
        logcacic->escrever(LogCacic::InfoLevel,QString("CacicInterface constructor."));
    } else {
        logcacic = new LogCacic(LOG_CACICINTERFACE, dirpath+"/Logs");
        logcacic->escrever(LogCacic::InfoLevel,QString("Deu pala. Else no construtor de CacicInterface."));
    }
}

CacicSysTray::~CacicSysTray()
{
    delete sysTray;
    delete logcacic;
}

void CacicSysTray::iniciarSysTray()
{
    if(QSystemTrayIcon::isSystemTrayAvailable()) {
        if(logcacic != NULL)
            logcacic->escrever(LogCacic::InfoLevel,QString("Inicializando SystemTray"));

        sysTray = new QSystemTrayIcon(this);
if(logcacic != NULL)
    logcacic->escrever(LogCacic::InfoLevel,QString("Objeto QSystemTrayIcon instanciado."));

        sysTray->setToolTip(QString("Cacic"));
if(logcacic != NULL)
logcacic->escrever(LogCacic::InfoLevel,QString("ToolTip setado."));
        sysTray->setIcon(QIcon(":/icon/cacic-logo.png"));
if(logcacic != NULL)
logcacic->escrever(LogCacic::InfoLevel,QString("Ícone setado."));

        QMenu *menu;
        QAction* action;

        menu = new QMenu();
        menu->addAction("generica");
        menu->addSeparator();
        action = new QAction(QString("alvo"),this);
        connect(action,&QAction::triggered,this,&CacicSysTray::actionClicked);
        menu->addAction(action);
        QMenu *newMenu = menu->addMenu("submenu");
if(logcacic != NULL)
    logcacic->escrever(LogCacic::InfoLevel,QString("Menus e actions inicializadas."));

        sysTray->setContextMenu(menu);
if(logcacic != NULL)
    logcacic->escrever(LogCacic::InfoLevel,QString("SetContextMenu feito."));

        sysTray->show();
if(logcacic != NULL)
    logcacic->escrever(LogCacic::InfoLevel,QString("SysTray showed."));

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
    if(actionPressed != 0)
        actionPressed->activate(QAction::Trigger);
}

void CacicSysTray::actionClicked()
{
    if(logcacic != NULL)
        logcacic->escrever(LogCacic::InfoLevel,QString("Systray action clicked."));
}
