#-------------------------------------------------
#
# Project created by QtCreator 2014-06-13T10:42:39
#
#-------------------------------------------------

QT += testlib
QT += network

TARGET = cacic-teste
CONFIG -= app_bundle
LIBS += -L/usr/lib -lcryptopp
TEMPLATE = app


SOURCES += \
    testcacic.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp

HEADERS += \
    testcacic.h \
    ../src/ccacic.h \
    ../src/CACIC_comm.h \
    ../src/cacic_computer.h
