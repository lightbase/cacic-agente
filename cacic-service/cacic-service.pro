#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T09:28:06
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cacic-service
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    cacicservice.cpp \
    cacicdaemon.cpp

include(../src/qtservice/src/qtservice.pri)

HEADERS += \
    cacicservice.h \
    cacicdaemon.h
