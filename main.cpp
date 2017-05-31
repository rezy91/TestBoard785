/***************************************************************************
**
** Copyright (C) 2016 Medical Technologies CZ a.s.. All rights reserved.
** Contact: http://www.medictech.com
**
** This file is a part of BTL088 project, projects based on BTL088
** or related utilities.
**
** Without prior written permission from Medical Technologies CZ a.s.,
** the file must not be modified, distributed and/or otherwise used.
**
****************************************************************************/

#include "mainwindow.h"
#include <QFontDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("btl_projects");
    QCoreApplication::setApplicationName(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));

    MainWindow o_main;

    QFontDatabase::addApplicationFont("/usr/lib/fonts/Vera.ttf");
    QGuiApplication::setFont(QFont("Vera",15));

    o_main.setWindowTitle(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));
    o_main.setWindowIcon(QIcon(":/iconMain.png"));
    o_main.show();

    return a.exec();
}
