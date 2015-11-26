#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, CACIC_SERVICE_NAME)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
    logcacic = new LogCacic(LOG_DAEMON, cacicMainFolder + "/Logs");
    this->createApplication(argc, argv);

}

cacicD::~cacicD()
{
    try{
        logcacic->~LogCacic();
        application()->exit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido no destrutor."));
    }
}

void cacicD::start() {
    try{
//        SocketListener *socket;
//        socket = new SocketListener(cacicMainFolder);
//        QObject::connect(socket, SIGNAL(forcaColeta()), Ocacictimer, SLOT(iniciarThread()));
        CacicTimer *Ocacictimer;
        Ocacictimer = new CacicTimer(cacicMainFolder);
//        QObject::connect(Ocacictimer, SIGNAL(finalizar()), this->application(), SLOT(quit()));
        QObject::connect(Ocacictimer, SIGNAL(finalizar()), this, SLOT(on_finalizar()));

#ifdef Q_OS_WIN
        //Força a atualização da versão do cacic no registro.
        CCacic::changeCacicVersion();
#endif

        logcacic->escrever(LogCacic::InfoLevel, QString("Cacic " + Identificadores::AGENTE_VERSAO + "."));
        Ocacictimer->iniciarTimer();
    }catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void cacicD::pause()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço pausado."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao pausar o serviço."));
    }
}

void cacicD::resume()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço resumido."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao resumir o serviço."));
    }
}

void cacicD::stop()
{
    try{
        ServiceClient *client = ServiceClient::Instance(cacicMainFolder,this);

        connect(this,SIGNAL(sendStopUiMsg()),client,SLOT(on_sendStopUiMsg()));
        connect(client,SIGNAL(uiStopped()),this,SLOT(on_uiStopped()));

        emit sendStopUiMsg();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao parar o serviço."));
    }
}

QByteArray cacicD::formatData(QString message, int messageLength)
{
    QByteArray data;
    data.append(QString::number(messageLength));
    data.append(" ");
    data.append(message);
    data.append(MSG_END);

    return data;
}

void cacicD::on_uiStopped()
{
    logcacic->escrever(LogCacic::InfoLevel, QString("Serviço parado."));
    this->application()->quit();
}

void cacicD::on_finalizar()
{
#ifdef Q_OS_WIN
    ServiceController *service = new ServiceController(QString(CHKSYS_NAME).toStdWString());
    if (service->isRunning()) {
        logcacic->escrever(LogCacic::InfoLevel, "CheckCacic rodando. Parando servico do mesmo.");
        if (!service->stop()) {
            std::string info = "Falha ao parar CheckCacic. Info: ";
            info+= service->getLastError();
            logcacic->escrever(LogCacic::ErrorLevel, info.c_str());
        }
    }
#else
                    ConsoleObject console;
                    if (CCacic::findProc("chksys")) console("killall -9 \"cacic-service\"");
#endif

    logcacic->escrever(LogCacic::InfoLevel, QString("Serviço parado."));
    this->application()->quit();
}
