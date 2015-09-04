#-------------------------------------------------
#
# Project Cacic Agente, module cacic-service, created by Lightbase
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 18
DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################
QT += testlib
QT += network
QT += widgets
QT -= gui
TARGET = cacic-teste

CONFIG -= app_bundle
CONFIG += c++11
TEMPLATE = app
win32 {
    QT += axcontainer
    LIBS += -lws2_32
    LIBS += -lshlwapi
    LIBS += -liphlpapi
#    LIBS += -LC:\Users\Cacic-Devel\Documents\Lightbase\cacic-agente\src\crypto++\lib -lcryptopp
} else {
#    LIBS += -L/usr/lib -lcryptopp
}

SOURCES += \
    testcacic.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp \
    ../src/cacic_comm.cpp \
    ../src/operatingsystem.cpp \
    ../src/cacic_software.cpp \
    ../src/cacic_hardware.cpp \
    ../src/vqtconvert.cpp \
    ../src/vregistry.cpp \
    ../src/ccoleta.cpp \
    ../src/wmi.cpp \
    ../src/QLogger.cpp \
    ../src/checkmodules.cpp \
    ../src/servicecontroller.cpp \
    ../src/identificadores.cpp \
    ../src/logcacic.cpp \
    ../src/wcomputer.cpp \
    ../src/cacicthread.cpp \
    ../src/cmodule.cpp \
    ../src/cmodulearray.cpp
#    ../src/qtservice/src/qtservice.cpp \
#    ../src/qtservice/src/qtservice_unix.cpp \
#    ../src/qtservice/src/qtservice_win.cpp \
#    ../src/qtservice/src/qtunixserversocket.cpp \
#    ../src/qtservice/src/qtunixsocket.cpp


HEADERS += \
    testcacic.h \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_comm.h \
    ../src/console.h \
    ../src/operatingsystem.h \
    ../src/cacic_software.h \
    ../src/cacic_hardware.h \
    ../src/vqtconvert.h \
    ../src/vregistry.h \
    ../src/ccoleta.h \
    ../src/wmi.h \
    ../src/QLogger.h \
    ../src/checkmodules.h \
    ../src/servicecontroller.h \
    ../src/identificadores.h \
    ../src/logcacic.h \
    ../src/wcomputer.h \
    ../src/cacicthread.h \
    ../src/cmodule.h \
    ../src/cmodulearray.h
#    ../src/qtservice/src/qtservice.h \
#    ../src/qtservice/src/qtservice_p.h \
#    ../src/qtservice/src/qtunixserversocket.h \
#    ../src/qtservice/src/qtunixsocket.h

INCLUDEPATH += ../src \
               ../src/crypto++/include/

include(../src/qtservice/src/qtservice.pri)

#RESOURCES += \
#    resource.qrc
