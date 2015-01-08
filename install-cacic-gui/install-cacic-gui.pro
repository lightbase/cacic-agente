#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T14:47:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = install-cacic-gui
TEMPLATE = app

INCLUDEPATH += ../src

SOURCES += main.cpp\
        installcacicgui.cpp \
        ../src/identificadores.cpp

HEADERS  += installcacicgui.h \
            ../src/identificadores.h

FORMS    += installcacicgui.ui
