/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef CACICMESSAGE_H
#define CACICMESSAGE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>
#include "logcacic.h"
#include "identificadores.h"

namespace Ui {
class CacicMessage;
}

class CacicMessage : public QMainWindow
{
    Q_OBJECT

public:
    explicit CacicMessage(QStringList args, QWidget *parent = 0);
    ~CacicMessage();

    QString getTitulo() const;
    void setTitulo(const QString &value);

    QString getMensagem() const;
    void setMensagem(const QString &value);

    QString getHash() const;
    void setHash(const QString &value);

private:
    Ui::CacicMessage *ui;
    void validaArgumentos(QStringList args);
    LogCacic *logcacic;
    QString titulo, mensagem, hash;

signals:
    void finished();

public slots:
    void exibeMensagem();

};

#endif // CACICMESSAGE_H
