#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T12:28:48
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

APPNAME = Master_785_simulator
MYVERSION = 0.0.6

TARGET = Master_785_simulator#"$$APPNAME $$MYVERSION"

CONFIG += c++11

DEFINES += APP_NAME=\\\"$$APPNAME\\\"
DEFINES += APP_VERSION=\\\"$$MYVERSION\\\"

CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    testpaint.cpp \
    widgetone.cpp \
    widgettwo.cpp \
    widgetthree.cpp

HEADERS  += mainwindow.h \
    testpaint.h \
    widgetone.h \
    widgettwo.h \
    widgetthree.h

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
