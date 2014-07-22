#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T16:13:12
#
#-------------------------------------------------

QT += core
QT += network
QT -= gui

TARGET = install-cacic
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    installcacic.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp

HEADERS += \
    installcacic.h \
    ../src/CACIC_comm.h \
    ../src/ccacic.h \
    ../src/cacic_computer.h
