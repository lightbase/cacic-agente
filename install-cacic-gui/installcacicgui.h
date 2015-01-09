#ifndef INSTALLCACICGUI_H
#define INSTALLCACICGUI_H

#include <QMainWindow>
#include <QFile>
#include "identificadores.h"
#include <QObject>
#include <QtCore>
#include <cacic_comm.h>
#include <cacic_computer.h>
#include <ccacic.h>
#include <stdlib.h>
#include <servicecontroller.h>
#include "iostream"
#include "QLogger.h"
#include <QTextCursor>


namespace Ui {
class InstallCacicGui;
}

class InstallCacicGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstallCacicGui(QWidget *parent = 0);
    ~InstallCacicGui();

    QMap<QString, QString> getArgumentos();
    void setArgumentos(QMap<QString, QString> value);
    QMap<QString, QString> validaParametros(QStringList argv, int argc, bool *ok);
    bool startProcess(QString pathprogram, bool wait, QStringList arguments);
    void registro();
    bool isGui() const;
    void setGui(bool value);
    bool verificarRoot();

private:
    Ui::InstallCacicGui *ui;
    bool verificarDadosInseridos();
    void instanciarObjetos();
    bool iniciarInstalacao(QStringList parametros);  
    void uninstall();
    QStringList listaDeParametros;
    QMap<QString, QString> argumentos;
    CacicComm *oCacicComm;
    CACIC_Computer oCacicComputer;
    CCacic oCacic;
    QLogger::QLoggerManager *logManager;
    QString applicationDirPath;
    bool gui;
    void parametrosIncorretos();
    void updateService();
    void configurar(const QMap<QString, QString> &param);
    void install();
    void mensagemDeProgresso(QString msg, bool limparMsgAnterior = false, bool primeiraLinha = false);

signals:
    void finished();

private slots:
    void on_pushButton_clicked();

public slots:
    void run(QStringList argv, int argc);

};

#endif // INSTALLCACICGUI_H
