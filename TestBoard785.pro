#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T12:28:48
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestAmp785
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    bytearrayparser.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../../../Applications/088
DEPENDPATH += $$PWD/../../../Applications/088

win32:CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../../../Applications/088/build-CommProtV200-win32-Debug/debug/ -lCommProtV200
}

RESOURCES += \
    resources.qrc

unix {
    target.path = /opt/btl
    INSTALLS += target
}
