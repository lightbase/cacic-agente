#-----------------------------------------------------------------------------------------
#
# Project Cacic Agente, module chksys, created by Lightbase
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-----------------------------------------------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 12
DEFINES += VERSION_MAJOR=$$VERSION_MAJOR \
           VERSION_MINOR=$$VERSION_MINOR \
           VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################

QT       += core
QT       -= gui
QT       -= widgets
QT       += network
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -fpermissive

CONFIG += debug_and_release

TARGET       = chksys

CONFIG      += console
CONFIG      -= app_bundle
CONFIG      += static

CONFIG      += c++11
TEMPLATE     = app
INCLUDEPATH += ../src \
#               ../../src/crypto++/include/

SOURCES     += main.cpp \
               chksys.cpp \
               ../src/ccacic.cpp \
               ../src/QLogger.cpp \
               ../src/identificadores.cpp \
               ../src/servicecontroller.cpp \
               ../src/logcacic.cpp \
               ../src/cacicthread.cpp \
               ../src/cacic_comm.cpp \
               chksystimer.cpp


HEADERS     += chksys.h \
               ../src/ccacic.h \
               ../src/QLogger.h \
               ../src/identificadores.h \
               ../src/servicecontroller.h \
               ../src/logcacic.h \
               ../src/cacicthread.h \
               ../src/cacic_comm.h \
               chksystimer.h

include(../src/qtservice/src/qtservice.pri)

