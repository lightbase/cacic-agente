#include "cacicsystray.h"
#include "cacicwidget.h"

CacicSysTray::CacicSysTray(const QString &dirpath, QWidget *parent) :
    QWidget(parent)
{
    if(!dirpath.isEmpty()) {
        mainFolder = dirpath;
        logcacic = new LogCacic(LOG_CACICUI, mainFolder+"/Logs");
    } else {
        logcacic = new LogCacic(LOG_CACICUI, Identificadores::ENDERECO_PATCH_CACIC+"/Logs");
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

        sysTray->setToolTip(QString("Cacic"));
        sysTray->setIcon(QIcon(":/icon/cacic-logo.png"));

        QMenu *menu;
        menu = setupMenu();
        sysTray->setContextMenu(menu);

        sysTray->show();
        if(logcacic != NULL && sysTray->isVisible())
            logcacic->escrever(LogCacic::InfoLevel,QString("SysTray iniciada."));

        connect(sysTray,&QSystemTrayIcon::activated,this,&CacicSysTray::on_activated);

    } else {
        if(logcacic != NULL)
            logcacic->escrever(LogCacic::InfoLevel,QString("Falha ao iniciar SystemTray"));
    }
}

QMenu* CacicSysTray::setupMenu()
{
    QMenu* menu;
    QAction* infos;
    QAction* forcarColeta;
    QAction* finalizar;

    menu = new QMenu();

    infos = new QAction(QString("Visualizar Informações"),this);
    connect(infos,&QAction::triggered,this,&CacicSysTray::infosClicked,Qt::UniqueConnection);
    menu->addAction(infos);

    forcarColeta = new QAction(QString("Forçar Coleta"),this);
    connect(forcarColeta,&QAction::triggered,this,&CacicSysTray::forcarClicked,Qt::UniqueConnection);
    menu->addAction(forcarColeta);

    menu->addSeparator();
    finalizar = new QAction(QString("Finalizar Serviço"),this);
    connect(finalizar,&QAction::triggered,this,&CacicSysTray::finalizarClicked,Qt::UniqueConnection);
    menu->addAction(finalizar);

    return menu;
}

void CacicSysTray::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "sysTrayClicked" << endl;
    QMenu* menu = sysTray->contextMenu();

    QAction* actionPressed = menu->exec(sysTray->geometry().topLeft());
    if(actionPressed != 0)
        actionPressed->activate(QAction::Trigger);
}
