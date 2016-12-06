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

/*!
  \file   main.cpp
  \author Pavel Hübner
  \date   22. 2. 2016
  \brief  main vytvářející GUI TestEncoder088
*/

#include "mainwindow.h"
#include <QFontDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("btl_projects");
    QCoreApplication::setApplicationName(APP_NAME);

    MainWindow w;

    QFontDatabase::addApplicationFont("/usr/lib/fonts/Vera.ttf");
    QGuiApplication::setFont(QFont("Vera",15));

    w.setWindowTitle(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));
    w.setWindowIcon(QIcon(":/appicon.ico"));
    w.show();

    return a.exec();
}
