#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include "cacicsystray.h"
#include "../src/ccacic.h"
#include "../src/logcacic.h"

namespace Ui {
class Widget;
}

class CacicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CacicWidget(QWidget *parent = 0);
    ~CacicWidget();
    bool isConnected();

private slots:
    void socketBytesWritten(qint64 bytes);
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();
    void setupWidget();

private:
    void setupSocketConnection();

    bool connected;
    LogCacic *logcacic;
    Ui::Widget *ui;
    QString cacicMainFolder;
    CacicSysTray *cacicSysTray;
    QTcpSocket *socket;
};

#endif // WIDGET_H
