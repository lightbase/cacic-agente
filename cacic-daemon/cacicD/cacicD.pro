#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T11:06:08
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network

TARGET = cacic-service
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += static
win32 {
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
    QT       += axcontainer
} else {
    LIBS += -L/usr/lib -lcryptopp
}
TEMPLATE = app
#TEMPLATE = lib

SOURCES += main.cpp \
           cacicd.cpp \
           cacictimer.cpp \
           ../../src/cacic_comm.cpp \
           ../../src/ccacic.cpp \
           ../../src/wmi.cpp \
           ../../src/cacic_computer.cpp \
           ../../src/operatingsystem.cpp \
           ../../src/QLogger.cpp

HEADERS += cacicd.h \
           cacictimer.h \
           ../../src/ccacic.h \
           ../../src/wmi.h \
           ../../src/cacic_computer.h \
           ../../src/operatingsystem.h \
           ../../src/cacic_comm.h \
           ../../src/QLogger.h

include(../../src/qtservice/src/qtservice.pri)

INCLUDEPATH += ../../src \
               ../../src/crypto++/include/

