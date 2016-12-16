#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T12:28:48
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

APPNAME = TestAmp785Messenger
VERSION = 0.1.0

TARGET = "$$APPNAME $$VERSION"

CONFIG += c++11

DEFINES += APP_NAME=\\\"$$APPNAME\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    settingstorage.cpp \
    settingstoragetestboard785.cpp

HEADERS  += mainwindow.h \
    bytearrayparser.h \
    settingstorage.h \
    settingstoragetestboard785.h

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
