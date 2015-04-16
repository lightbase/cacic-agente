#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T16:13:12
#
#-------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 11
DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################
CONFIG += debug_and_release
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

#    win32 {
#        LIBS += -LC:\Devel\cacic-agente\src\crypto++\lib -lcryptopp
        QT += axcontainer
#    } else {
#        DEPENDPATH += ../lib
#        LIBS += ../lib/libcryptopp.a
        LIBS += ../lib/libQt5Core.a
        LIBS += ../lib/libQt5Network.a
#    }

} else {
    win32 {
#        LIBS += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
        QT += axcontainer
    } else {
#        LIBS += -L/usr/lib -lcryptopp
    }
}

#TEMPLATE = app

SOURCES +=  main.cpp \
            installcacic.cpp \
            ../src/ccacic.cpp \
            ../src/cacic_comm.cpp \
            ../src/cacic_computer.cpp \
            ../src/operatingsystem.cpp \
            ../src/wmi.cpp \
            ../src/identificadores.cpp \
            ../src/servicecontroller.cpp \
            ../src/QLogger.cpp \
            ../src/logcacic.cpp

HEADERS += \
            installcacic.h \
            ../src/ccacic.h \
            ../src/cacic_computer.h \
            ../src/cacic_comm.h \
            ../src/operatingsystem.h \
            ../src/wmi.h \
            ../src/console.h \
            ../src/identificadores.h \
            ../src/servicecontroller.h \
            ../src/QLogger.h \
            ../src/logcacic.h

INCLUDEPATH += ../src \
#               ../src/crypto++/include/
