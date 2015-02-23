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
#include <QMessageBox>
#include <QLineEdit>
#include "../install-cacic/installcacic.h"
#include <QSystemTrayIcon>

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
    int getModoDeExecucao() const;
    void setModoDeExecucao(int value);
    void setInstallC(InstallCacic *value);

private:
    Ui::InstallCacicGui *ui;
    bool verificarDadosInseridos();
    bool iniciarInstalacao(QStringList parametros);
    void uninstall();
    QSystemTrayIcon *tray;
    QMenu *trayMenu;
    QStringList listaDeParametros;
    QMap<QString, QString> argumentos;
    CacicComm *oCacicComm;
    CACIC_Computer oCacicComputer;
    CCacic oCacic;
    InstallCacic *installC;
    QLogger::QLoggerManager *logManager;
    QString applicationDirPath;
    int modoDeExecucao;
    bool gui;
    void parametrosIncorretos();
    void updateService();
    void configurar(const QMap<QString, QString> &param);
    void install();
    void mensagemDeProgresso(QString msg, bool limparMsgAnterior = false, bool primeiraLinha = false);
    void resolverModoDeExecucao();
    void resolverAcoesAoSelecionarCheckBox(int arg1, QLineEdit *le, QString registro);    
    void trayIcon(bool addMenu, QString menu, bool sendMsg, QString titulo, QString msg);

signals:
    void finished();

private slots:
    void on_pbInstalar_clicked();

    void on_cbHost_stateChanged(int arg1);

    void on_cbUsu_stateChanged(int arg1);

    void on_cbPass_stateChanged(int arg1);

    void on_pbCancelar_clicked();

public slots:
    void run(QStringList argv, int argc);
};

#endif // INSTALLCACICGUI_H
