#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T16:13:12
#
#-------------------------------------------------

QT -= core
QT += network
QT -= gui
LIBS -= -lQtGui
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter

TARGET = install-cacic
#CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

release {
#Descomentar essas linhas se quiser bibliotecas estáticas
#    CONFIG += static
#    CONFIG += staticlib
#    message("Release build.")
}

static {
    DEFINES += STATIC
    message("Static build.")

    win32 {
#        LIBS += -LC:\Devel\cacic-agente\src\crypto++\lib -lcryptopp
        QT += axcontainer
    } else {
        DEPENDPATH += ../lib
        LIBS += ../lib/libcryptopp.a
        LIBS += ../lib/libQt5Core.a
        LIBS += ../lib/libQt5Network.a
    }

} else {
    win32 {
#        LIBS += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
        QT += axcontainer
    } else {
        LIBS += -L/usr/lib -lcryptopp
    }
}

#TEMPLATE = app

SOURCES += main.cpp \
    installcacic.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_comm.cpp \
    ../src/cacic_computer.cpp \
    ../src/operatingsystem.cpp \
    ../src/wmi.cpp \
    ../src/QLogger.cpp \
    ../src/identificadores.cpp

HEADERS += \
    installcacic.h \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_comm.h \
    ../src/operatingsystem.h \
    ../src/wmi.h \
    ../src/QLogger.h \
    ../src/console.h \
    ../src/identificadores.h

INCLUDEPATH += ../src \
#               ../src/crypto++/include/
