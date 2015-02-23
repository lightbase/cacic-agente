#ifndef NOROOT_H
#define NOROOT_H

#include <QMainWindow>

namespace Ui {
class NoRoot;
}

class NoRoot : public QMainWindow
{
    Q_OBJECT

public:
    explicit NoRoot(QWidget *parent = 0);
    ~NoRoot();

private slots:
    void on_btFechar_clicked();

signals:
    void finished();

private:
    Ui::NoRoot *ui;
};

#endif // NOROOT_H
