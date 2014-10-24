#-------------------------------------------------
#
# Project created by Lightbase 2014-08-18T11:06:08
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network

TARGET       = cacic-service
CONFIG      += console
CONFIG      -= app_bundle
CONFIG      += static
TEMPLATE     = app
INCLUDEPATH += ../../src \
               ../../src/crypto++/include/

win32 {
    LIBS    += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
    QT      += axcontainer
} else {
    LIBS    += -L/usr/lib -lcryptopp
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
               ../../src/checkmodules.cpp


HEADERS     += cacicd.h \
               cacictimer.h \
               cacicthread.h \
               ../../src/ccacic.h \
               ../../src/wmi.h \
               ../../src/cacic_computer.h \
               ../../src/operatingsystem.h \
               ../../src/cacic_comm.h \
               ../../src/QLogger.h \
               ../../src/checkmodules.h

include(../../src/qtservice/src/qtservice.pri)

