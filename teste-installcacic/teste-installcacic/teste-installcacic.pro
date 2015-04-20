#-------------------------------------------------
#
# Project Cacic Agente, module cacic-service, created by Lightbase
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-------------------------------------------------

QT += testlib
QT += network
QT -= gui
TARGET = teste-installcacic

CONFIG -= app_bundle

TEMPLATE = app

SOURCES += testeinstallcacic.cpp \
    ../../installcacic-standalone/main.cpp \
    ../../installcacic-standalone/installcacicsa.cpp
HEADERS += testeinstallcacic.h \
    ../../installcacic-standalone/installcacicsa.h

