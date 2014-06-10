#-------------------------------------------------
#
# Project created by QtCreator 2014-06-10T15:14:24
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT += testlib

TARGET = cacic-agente

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += testcase

TEMPLATE = app


SOURCES += main.cpp \
    testservice.cpp \
    testget.cpp

HEADERS += \
    testservice.h \
    testget.h
