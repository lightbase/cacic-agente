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
VERSION_BUILD = 13
DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################

QT += testlib
QT += network
QT -= gui
TARGET = teste-installcacic

CONFIG -= app_bundle

TEMPLATE = app

SOURCES += testeinstallcacic.cpp \
    ../installcacic-standalone/installcacicsa.cpp \
    ../src/ccacic.cpp \
    ../src/identificadores.cpp \
    ../src/servicecontroller.cpp

HEADERS += testeinstallcacic.h \
    ../installcacic-standalone/installcacicsa.h \
    ../src/ccacic.h \
    ../src/identificadores.h \
    ../src/servicecontroller.h

INCLUDEPATH += ../src
