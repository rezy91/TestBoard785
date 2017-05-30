#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T12:28:48
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

APPNAME = Master_785_simulator
MYVERSION = 0.1.1

TARGET = Master_785_simulator#"$$APPNAME $$MYVERSION"

CONFIG += c++11

DEFINES += APP_NAME=\\\"$$APPNAME\\\"
DEFINES += APP_VERSION=\\\"$$MYVERSION\\\"

CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    widgetconfig.cpp \
    widgetreading.cpp \
    widgetgraph.cpp \
    widgetsmith.cpp \
    widgettherapy.cpp \
    settings.cpp \
    widgetsettings.cpp

HEADERS  += mainwindow.h \
    widgetconfig.h \
    widgetreading.h \
    widgetgraph.h \
    widgetsmith.h \
    common.h \
    widgettherapy.h \
    settings.h \
    widgetsettings.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../../../Applications/088
DEPENDPATH += $$PWD/../../../Applications/088

win32 {
    RC_FILE = app.rc
}

RESOURCES += \
    resources.qrc

unix {
    target.path = /opt/btl
    INSTALLS += target
}


U_LIBS += CommProtV200
U_LIB_DIR_PREFIX += ../../../Applications/088/build-CommProtV200
include(../../../Applications/088/BuildUtilities/scripts/lib_setup.pri)
