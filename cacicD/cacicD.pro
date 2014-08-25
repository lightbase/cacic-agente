#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T11:06:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cacicD
CONFIG   += console
CONFIG   -= app_bundle
win32 {
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
} else {
    LIBS += -L/usr/lib -lcryptopp
}
TEMPLATE = app


SOURCES += main.cpp \
    ../src/ccacic.cpp \
    cacicd.cpp

HEADERS += \
    ../src/ccacic.h \
    cacicd.h

include(../src/qtservice/src/qtservice.pri)

INCLUDEPATH += ../src \
               ../src/crypto++/include/
