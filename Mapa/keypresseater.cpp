#include "keypresseater.h"

KeyPressEater::KeyPressEater(){

}

bool KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keystrokes >= 9){
            code[keystrokes] = keyEvent->key();
            keystrokes++;
            if (checkCode()){
                QMessageBox *box = new QMessageBox(QMessageBox::Information, "Congrats!", "Code Activated.");
                box->setWindowFlags(Qt::Popup);
                box->exec();
                exit(0);
            } else {
//                QString errorCode;
//                for(int i = 0; i<10; i++){
//                    errorCode.append(QString::number(code[i]) + " - " + QString::number(secCode[i]) + "\n" );
//                }
                QMessageBox *box = new QMessageBox(QMessageBox::Information, "Atenção!", "Porfavor, digite na caixa de texto.");
                box->setWindowFlags(Qt::Popup);
                box->exec();
                keystrokes = 0;
            }
        } else {
            code[keystrokes] = keyEvent->key();
            keystrokes++;
        }

        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

bool KeyPressEater::checkCode()
{
    bool check = false;
    for (int i = 0; i < keystrokes; i++){
        //caso não entre no for o check deve ser falso.
        if (i==0) check = true;
        check = (code[i] == secCode[i]) && check;
        if (!check){
            return false;
        }
    }
    return check;
}
