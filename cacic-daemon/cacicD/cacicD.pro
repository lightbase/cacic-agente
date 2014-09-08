#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T11:06:08
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network
QT       += axcontainer
TARGET = cacic-service
CONFIG   += console
CONFIG   -= app_bundle
win32 {
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
} else {
    LIBS += -L/usr/lib -lcryptopp
}
TEMPLATE = app


SOURCES += main.cpp \
    ../../src/ccacic.cpp \
    cacicd.cpp \
    ../../src/wmi.cpp \
    cacictimer.cpp \
    ../../src/cacic_computer.cpp \
    ../../src/operatingsystem.cpp

HEADERS += \
    ../../src/ccacic.h \
    cacicd.h \
    cacictimer.h \
    ../../src/wmi.h \
    ../../src/cacic_computer.h \
    ../../src/operatingsystem.h \
    ../../src/cacic_comm.h

include(../../src/qtservice/src/qtservice.pri)

INCLUDEPATH += ../../src \
               ../../src/crypto++/include/

