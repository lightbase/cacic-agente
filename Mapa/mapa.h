/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/

#ifndef MAPA_H
#define MAPA_H

#include <QMainWindow>
#include <QtCore>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include "cacic_comm.h"
#include "ccacic.h"
#include "cacic_computer.h"
#include "logcacic.h"
#include "identificadores.h"
#include "sys/time.h"
#include "string"
#include "ui_mapa_default.h"
#include "ldaphandler.h"

namespace Ui {
class Mapa;
}

class Mapa : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mapa(QWidget *parent = 0);
    explicit Mapa(const bool &consultaLdap, QWidget *parent = 0);
    ~Mapa();
    void setComm(const QString &server);

    void inicializarAtributos();
private slots:
    void on_okButton_clicked();

private:
    bool checarPreenchimento() const;
    void closeEvent(QCloseEvent *event);
    bool enviarInfo(const QJsonObject &jsonMapa);
    void preencheCampos(bool consultaLdap);
    bool preencheNomeUsuario();
    bool validarCampos(QList< QPair<QString,QString> > &listaValores);

    CACIC_Computer computer;
    CacicComm *oCacicComm;
    LogCacic *logcacic;
    QString mainFolder;
    Ui::Mapa *ui;
signals:
    void finished();
};

#endif // MAPA_H
