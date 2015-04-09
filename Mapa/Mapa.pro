#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T12:50:22
#
#-------------------------------------------------
##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 9
DEFINES += VERSION_MAJOR=$$VERSION_MAJOR \
           VERSION_MINOR=$$VERSION_MINOR \
           VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mapa
TEMPLATE = app

INCLUDEPATH += ../src

SOURCES += main.cpp\
        mapa.cpp\
        ../src/cacic_comm.cpp\
        ../src/ccacic.cpp\
        ../src/cacic_computer.cpp\
        ../src/operatingsystem.cpp\
        ../src/QLogger.cpp\
        ../src/logcacic.cpp\
        ../src/identificadores.cpp

HEADERS  += mapa.h\
        ../src/cacic_comm.h\
        ../src/ccacic.h\
        ../src/cacic_computer.h\
        ../src/operatingsystem.h\
        ../src/QLogger.h\
        ../src/logcacic.h\
        ../src/identificadores.h

FORMS    += \
    mapa_default.ui

RESOURCES += \
    mapa.qrc
