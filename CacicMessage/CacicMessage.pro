#-----------------------------------------------------------------------------------------
#
# Project Cacic Agente, module cacic-service, created by Lightbase
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-----------------------------------------------------------------------------------------

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
@CONFIG += debug_and_release@
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CacicMessage
TEMPLATE = app

INCLUDEPATH += ../src \

SOURCES += main.cpp \
           cacicmessage.cpp \
           ../src/QLogger.cpp \
           ../src/logcacic.cpp \
           ../src/identificadores.cpp

HEADERS  += cacicmessage.h \
            ../src/QLogger.h \
            ../src/logcacic.h \
            ../src/identificadores.h

FORMS    += cacicmessage.ui

win32 {
    RC_ICONS = cacic-logo.ico
}
