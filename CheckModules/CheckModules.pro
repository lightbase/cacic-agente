#-------------------------------------------------
#
# Project created by QtCreator 2014-10-17T21:00:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CheckModules
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp \
    ../src/cacic_comm.cpp \
    ../src/operatingsystem.cpp \
    ../src/wmi.cpp \
    ../src/checkmodules.cpp


HEADERS += \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_comm.h \
    ../src/operatingsystem.h \
    ../src/wmi.h \
    ../src/checkmodules.h
