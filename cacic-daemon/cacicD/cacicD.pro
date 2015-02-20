#-------------------------------------------------
#
# Project created by Lightbase 2014-08-18T11:06:08
#
#-------------------------------------------------

#SEMPRE MUDAR A VERSÃO DE BUILD (o quarto número) AO REALIZAR QUALQUER BUILD.#
VERSION   = 3.1.8a
##############################################################################

QT       += core
QT       -= gui
QT       -= widgets
QT       += network
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -fpermissive

TARGET       = cacic-service

CONFIG      += console
CONFIG      -= app_bundle
CONFIG      += static
CONFIG      += c++11
TEMPLATE     = app
INCLUDEPATH += ../../src \
#               ../../src/crypto++/include/

win32 {
#    LIBS    += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
    QT      += axcontainer
} else {
#    LIBS    += -L/usr/lib -lcryptopp
}

SOURCES     += main.cpp \
               cacicd.cpp \
               cacictimer.cpp \
               cacicthread.cpp \
               ../../src/cacic_comm.cpp \
               ../../src/ccacic.cpp \
               ../../src/wmi.cpp \
               ../../src/cacic_computer.cpp \
               ../../src/operatingsystem.cpp \
               ../../src/QLogger.cpp \
               ../../src/checkmodules.cpp \
               ../../src/identificadores.cpp \
               ../../src/servicecontroller.cpp \
               socketlistener.cpp


HEADERS     += cacicd.h \
               cacictimer.h \
               cacicthread.h \
               ../../src/ccacic.h \
               ../../src/wmi.h \
               ../../src/cacic_computer.h \
               ../../src/operatingsystem.h \
               ../../src/cacic_comm.h \
               ../../src/QLogger.h \
               ../../src/checkmodules.h \
               ../../src/identificadores.h \
               ../../src/servicecontroller.h \
               socketlistener.h

include(../../src/qtservice/src/qtservice.pri)

