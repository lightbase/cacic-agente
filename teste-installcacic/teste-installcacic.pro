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
VERSION_BUILD = 14
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

LIBS += -lws2_32

TEMPLATE = app

SOURCES += testeinstallcacic.cpp \
    ../installcacic-standalone/installcacicsa.cpp \
    ../installcacic-standalone/commsa.cpp \
    ../src/ccacic.cpp \
    ../src/identificadores.cpp \
    ../src/servicecontroller.cpp \
#    ../src/greta/regexpr2.cpp \
#    ../src/greta/syntax2.cpp \
#    ../src/greta/restack.cpp \
#    ../src/greta/reimpl2.cpp

HEADERS += testeinstallcacic.h \
    ../installcacic-standalone/installcacicsa.h \
    ../installcacic-standalone/commsa.h \
    ../src/ccacic.h \
    ../src/identificadores.h \
    ../src/servicecontroller.h \
#    ../src/greta/regexpr2.h \
#    ../src/greta/syntax2.h \
#    ../src/greta/restack.h \
#    ../src/greta/reimpl2.h

INCLUDEPATH += ../src
