#ifndef KEYPRESSEATER_H
#define KEYPRESSEATER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>

class KeyPressEater : public QObject
{
    Q_OBJECT

public:
    KeyPressEater();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    int code[10];
    int keystrokes = 0;
    int secCode[10] = {16777235, 16777235, 16777237, 16777237, 16777234, 16777236, 16777234, 16777236, 66, 65};

    bool checkCode();
};

#endif // KEYPRESSEATER_H
