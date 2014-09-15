#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T16:13:12
#
#-------------------------------------------------

QT += core
QT += network
QT -= gui

TARGET = install-cacic
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

win32 {
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
    QT += axcontainer
} else {
    LIBS += -L/usr/lib -lcryptopp
}

TEMPLATE = app

SOURCES += main.cpp \
    installcacic.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp \
    ../src/operatingsystem.cpp \
    ../src/wmi.cpp \
    ../src/QLogger.cpp

HEADERS += \
    installcacic.h \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_comm.h \
    ../src/operatingsystem.h \
    ../src/wmi.h \
    ../src/QLogger.h

INCLUDEPATH += ../src \
               ../src/crypto++/include/
