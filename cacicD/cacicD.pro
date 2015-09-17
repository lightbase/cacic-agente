#-----------------------------------------------------------------------------------------
#
# Project Cacic Agente, module cacic-service, created by Lightbase (http://www.softwarepublico.gov.br/ver-comunidade?community_id=3585)
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-----------------------------------------------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 19
DEFINES += VERSION_MAJOR=$$VERSION_MAJOR \
           VERSION_MINOR=$$VERSION_MINOR \
           VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################
CONFIG += debug_and_release
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
INCLUDEPATH += ../src \
#               ../../src/crypto++/include/

win32 {
#    LIBS    += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
    QT      += axcontainer
    LIBS += -lws2_32
    LIBS += -lshlwapi
    LIBS += -liphlpapi
} else {
#    LIBS    += -L/usr/lib -lcryptopp
}

SOURCES     += main.cpp \
               cacicd.cpp \
               cacictimer.cpp \
               ../src/cacicthread.cpp \
               ../src/cacic_comm.cpp \
               ../src/ccacic.cpp \
               ../src/wmi.cpp \
               ../src/cacic_computer.cpp \
               ../src/operatingsystem.cpp \
               ../src/QLogger.cpp \
               ../src/checkmodules.cpp \
               ../src/identificadores.cpp \
               ../src/servicecontroller.cpp \
               socketlistener.cpp \
               ../src/logcacic.cpp \
                ../src/vregistry.cpp \
                ../src/vqtconvert.cpp \
                ../src/wcomputer.cpp \
                ../src/cmodulearray.cpp \
                ../src/cmodule.cpp \
                winprocess.cpp


HEADERS     += cacicd.h \
               cacictimer.h \
               ../src/cacicthread.h \
               ../src/ccacic.h \
               ../src/wmi.h \
               ../src/cacic_computer.h \
               ../src/operatingsystem.h \
               ../src/cacic_comm.h \
               ../src/QLogger.h \
               ../src/checkmodules.h \
               ../src/identificadores.h \
               ../src/servicecontroller.h \
               socketlistener.h \
               ../src/logcacic.h \
               ../src/vregistry.h \
               ../src/vqtconvert.h \
               ../src/wcomputer.h \
                ../src/cmodulearray.h \
                ../src/cmodule.h \
                winprocess.h

include(../src/qtservice/src/qtservice.pri)

