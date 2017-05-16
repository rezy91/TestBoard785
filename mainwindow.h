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
  \file   mainwindow.h
  \author Pavel Hübner
  \date   22. 2. 2016
  \brief  GUI zasílající a přijímající zprávy generu
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include "../../../Applications/088/CommProtV200/commprotv200_global.h"
#include <QSettings>
#include <QDesktopServices>

#include <QtGui>
#include <QtCore>
#include <QPushButton>


#include "widgetone.h"
#include "widgettwo.h"
#include "widgetthree.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void changed_table(int index);

private:

    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    quint32 m_nDeviceAddress;
    QTimer TmrMstr;
    QTime timeCurrent;



    widgetOne *p_WidgetConfig = new widgetOne(this);
    widgetTwo *p_WidgetReading = new widgetTwo(this);
    widgetThree *p_WidgetSettings = new widgetThree(this);


    void newDataV200(QByteArray aData);
    void refreshPlot(void);



protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
