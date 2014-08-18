#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T11:06:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cacicD
CONFIG   += console
CONFIG   -= app_bundle
win32 {
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
} else {
    LIBS += -L/usr/lib -lcryptopp
}
TEMPLATE = app


SOURCES += main.cpp \
    ../src/cacic_computer.cpp \
    ../src/ccacic.cpp \
    ../src/ccoleta.cpp \
    ../src/operatingsystem.cpp \
    cacicd.cpp

HEADERS += \
    ../src/cacic_comm.h \
    ../src/cacic_computer.h \
    ../src/ccacic.h \
    ../src/ccoleta.h \
    ../src/console.h \
    ../src/operatingsystem.h \
    cacicd.h

include(../src/qtservice/src/qtservice.pri)
