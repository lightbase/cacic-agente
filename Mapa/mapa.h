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
#include "ccacic.h"
#include "cacic_computer.h"
#include "logcacic.h"
#include "identificadores.h"
#include "sys/time.h"
#include "string"
#include "ui_mapa_default.h"
#include "ldaphandler.h"
#include "keypresseater.h"
#include "novocampo.h"
#include <QListView>

namespace Ui {
    class Mapa;
}

class Mapa : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mapa(const QJsonObject &getMapaJson, QWidget *parent = 0);
    ~Mapa();

    void inicializarAtributos(const QJsonObject &getMapaJson);
private slots:
    void on_okButton_clicked();

private:
    bool checarPreenchimento() const;
    void closeEvent(QCloseEvent *event);
    void geraCampoMensagem();
    void geraNovosCampos();
    bool salvarInfo(const QJsonObject &jsonMapa);
    void preencheCampos();
    QString preencheNomeUsuario();
    bool validarCampos(QList< QPair<QString,QString> > &listaValores);

    CACIC_Computer computer;
    LogCacic *logcacic;
    QJsonObject mapaJson;
    QList<NovoCampo> listNovosCampos;
    QString mainFolder;
    Ui::Mapa *ui;
    KeyPressEater *keyPressEater ;
signals:
    void finished();
};


#endif // MAPA_H
