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
VERSION_MINOR = 2
VERSION_BUILD = 0
DEFINES += VERSION_MAJOR=$$VERSION_MAJOR \
           VERSION_MINOR=$$VERSION_MINOR \
           VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################
CONFIG += debug_and_release
QT       += core gui network
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mapacacic
TEMPLATE = app

win32 {
#    LIBS    += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
    LIBS += -lwldap32
    QT      += axcontainer
    LIBS += -lws2_32
    LIBS += -liphlpapi
} else {
#    LIBS    += -L/usr/lib -lcryptopp
    LIBS += -lldap
}

INCLUDEPATH += ../src

SOURCES += main.cpp\
        mapa.cpp\
        ../src/ccacic.cpp\
        ../src/cacic_computer.cpp\
../src/cacic_comm.cpp\
        ../src/operatingsystem.cpp\
        ../src/QLogger.cpp\
        ../src/logcacic.cpp\
        ../src/identificadores.cpp\
        ../src/wmi.cpp \
        ../src/servicecontroller.cpp \
        mapacontrol.cpp \
        ../src/ldaphandler.cpp \
        ../src/vregistry.cpp \
        ../src/vqtconvert.cpp \
        ../src/wcomputer.cpp \
        keypresseater.cpp \
        novocampo.cpp

HEADERS  += mapa.h\
        ../src/ccacic.h\
        ../src/cacic_computer.h\
../src/cacic_comm.h\
        ../src/operatingsystem.h\
        ../src/QLogger.h\
        ../src/logcacic.h\
        ../src/identificadores.h\
        ../src/wmi.h \
        ../src/servicecontroller.h \
        mapacontrol.h \
        ../src/ldaphandler.h \
        ../src/vregistry.h \
        ../src/vqtconvert.h \
        ../src/wcomputer.h \
        keypresseater.h \
        novocampo.h

FORMS    += \
    mapa_default.ui

RESOURCES += \
    mapa.qrc
