#-------------------------------------------------
#
# Project created by QtCreator 2015-04-22T17:30:43
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

TARGET = http-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    http-server.cpp \
    http-daemon.cpp

HEADERS += \
    http-server.h \
    http-daemon.h

INCLUDEPATH += ../src

include(../src/qtservice/src/qtservice.pri)
