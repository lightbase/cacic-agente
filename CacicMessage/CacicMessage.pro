#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T10:45:49
#
#-------------------------------------------------
##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 10
DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################

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
