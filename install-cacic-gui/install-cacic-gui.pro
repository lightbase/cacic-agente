#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T14:47:27
#
#-------------------------------------------------

QT       += core gui network
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -fpermissive
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = install-cacic-gui
TEMPLATE = app

static {
    DEFINES += STATIC
    message("Static build.")
        QT += axcontainer
        LIBS += ../lib/libQt5Core.a
        LIBS += ../lib/libQt5Network.a
} else {
    win32 {
        QT += axcontainer
    }
}



INCLUDEPATH += ../src \

SOURCES +=  main.cpp\
            installcacicgui.cpp \
            ../src/ccacic.cpp \
            ../src/cacic_comm.cpp \
            ../src/cacic_computer.cpp \
            ../src/operatingsystem.cpp \
            ../src/wmi.cpp \
            ../src/QLogger.cpp \
            ../src/identificadores.cpp \
            ../src/servicecontroller.cpp \
    noroot.cpp \
    uninstallcacic.cpp

HEADERS  += installcacicgui.h \
            ../src/ccacic.h \
            ../src/cacic_computer.h \
            ../src/cacic_comm.h \
            ../src/operatingsystem.h \
            ../src/wmi.h \
            ../src/QLogger.h \
            ../src/console.h \
            ../src/identificadores.h \
    noroot.h \
    uninstallcacic.h
            ../src/servicecontroller.h

FORMS    += installcacicgui.ui \
    noroot.ui \
    uninstallcacic.ui

win32 {
    RC_ICONS = cacic-logo.ico
} else {
    RC_ICONS = cacic-logo.png
}
