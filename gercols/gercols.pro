#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T13:28:50
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET    = gercols

TEMPLATE = app

SOURCES += \
    main.cpp \
    ./gercols.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp \
    ../src/cacic_software.cpp \
    ../src/cacic_hardware.cpp \
    ../src/operatingsystem.cpp \
    ../src/ccoleta.cpp \
    ../src/vregistry.cpp \
    ../src/vqtconvert.cpp \
    ../src/wmi.cpp \

HEADERS += \
    ./gercols.h \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_software.h \
    ../src/cacic_hardware.h \
    ../src/operatingsystem.h \
    ../src/console.h \
    ../src/ccoleta.h \
    ../src/vregistry.h \
    ../src/vqtconvert.h \
    ../src/wmi.h \

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += qt
CONFIG   += c++11
CONFIG += static

win32 {
    QT   += axcontainer
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
} else {
    LIBS += -L/usr/lib -lcryptopp
}



INCLUDEPATH += ../src \
               ../src/crypto++/include/

DEFINES += STATIC
