#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T13:28:50
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET    = gercols
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += qt
CONFIG   += c++11

win32 {
    QT   += axcontainer
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
} else {
    LIBS += -L/usr/lib -lcryptopp
}

TEMPLATE = app

SOURCES += \
    main.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp \
    ../src/cacic_software.cpp \
    ../src/cacic_hardware.cpp \
    ../src/operatingsystem.cpp \
    ../src/ccoleta.cpp \
    ./gercols.cpp \
    ../src/vregistry.cpp \
    ../src/vqtconvert.cpp \

HEADERS += \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_software.h \
    ../src/cacic_hardware.h \
    ../src/operatingsystem.h \
    ../src/console.h \
    ../src/ccoleta.h \
    ./gercols.h \
    ../src/vregistry.h \
    ../src/vqtconvert.h \

INCLUDEPATH += ../src \
               ../src/crypto++/include/
