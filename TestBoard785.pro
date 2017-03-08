#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T12:28:48
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

APPNAME = Master_785_simulator
VERSION = 0.0.2

TARGET = "$$APPNAME $$VERSION"

CONFIG += c++11

DEFINES += APP_NAME=\\\"$$APPNAME\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    settings.cpp \
    smithmain.cpp \
    grapmain.cpp

HEADERS  += mainwindow.h \
    settings.h \
    smithmain.h \
    grapmain.h \
    common.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../../../Applications/088
DEPENDPATH += $$PWD/../../../Applications/088

win32 {
    RC_FILE = app.rc
}

win32:CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../../../Applications/088/build-CommProtV200-win32-Debug/debug/ -lCommProtV200
}

win32:CONFIG(release, debug|release) {
    LIBS += -L$$PWD/../../../Applications/088/build-CommProtV200-win32-Release/release/ -lCommProtV200
}

RESOURCES += \
    resources.qrc

unix {
    target.path = /opt/btl
    INSTALLS += target
}
