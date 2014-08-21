#ifndef CACICD_H
#define CACICD_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "qtservice.h"


class cacicD : public QtService<QCoreApplication>
{
public:
    /**
     * @brief cacicD Construtor
     * @param argc
     * @param argv
     */
    cacicD(int argc, char **argv);
    /**
     * @brief cacicD Desconstrutor
     */
    ~cacicD();
    /**
     * @brief inicia o serviço
     */
    void start();
    /**
     * @brief pausa o serviço
     */
    void pause();
    /**
     * @brief resume a execução do serviço
     */
    void resume();
    /**
     * @brief para a execução do serviço
     */
    void stop();


};

#endif // CACICD_H
