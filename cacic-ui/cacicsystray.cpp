#include "cacicsystray.h"
#include "cacicwidget.h"

CacicSysTray::CacicSysTray(const QString &dirpath, QTcpSocket * const socket, QWidget *parent) :
    QWidget(parent), socket(socket)
{
    if(!dirpath.isEmpty()) {
        this->mainFolder = dirpath;
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

        connect(sysTray,&QSystemTrayIcon::activated,this,&CacicSysTray::sysTrayClicked);

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
    connect(infos,&QAction::triggered,this,&CacicSysTray::infosClicked);
    menu->addAction(infos);

    forcarColeta = new QAction(QString("Forçar Coleta"),this);
    connect(forcarColeta,&QAction::triggered,this,&CacicSysTray::forcarClicked);
    menu->addAction(forcarColeta);

    menu->addSeparator();
    finalizar = new QAction(QString("Finalizar Serviço"),this);
    connect(finalizar,&QAction::triggered,this,&CacicSysTray::finalizarClicked);
    menu->addAction(finalizar);

    return menu;
}

void CacicSysTray::sysTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
    qDebug() << "sysTrayClicked" << endl;
    QMenu* menu = sysTray->contextMenu();

    QAction* actionPressed = menu->exec(sysTray->geometry().topLeft());
    if(actionPressed != 0)
        actionPressed->activate(QAction::Trigger);
}

void CacicSysTray::infosClicked()
{
    //TODO: Aqui irei ter que criar uma janela com detalhes da coleta e do chksis
    emit infosClicked();
}

void CacicSysTray::forcarClicked()
{
    // TODO: enviar comando para forçar coleta no serviço
    if(static_cast<CacicWidget*>(parent())->isConnected())
        socket->write("forcarClicked");
    else
        logcacic->escrever(LogCacic::ErrorLevel,QString("forcarClicked: socket disconnected."));

}

void CacicSysTray::finalizarClicked()
{
    // TODO: enviar comando para finalizar os serviços do Cacic
    if(static_cast<CacicWidget*>(parent())->isConnected())
        socket->write("finalizarClicked");
    else
        logcacic->escrever(LogCacic::ErrorLevel,QString("finalizarClicked: socket disconnected."));
}

