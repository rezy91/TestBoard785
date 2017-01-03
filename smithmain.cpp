#include "smithmain.h"
#include "ui_mainwindow.h"

#include <QLabel>

#include <QtGui>
#include <QtCore>

SmithMain::SmithMain(QWidget *parent) : QMainWindow(parent)
{
    setFixedHeight(900);
    setFixedWidth(900);

    setStyleSheet("background-image: url(smith.png)");
}


void SmithMain::paintEvent(QPaintEvent *e)
{
    static bool forFirst = false;

    QPainter painter;
    QRectF rectangle1(450.0, 450.0, 10.0, 10.0);
    QRectF rectangle2(250.0, 250.0, 10.0, 10.0);
    QPen pen;

    /*if(!forFirst)
    {*/
        forFirst = true;
        painter.begin(this);
    //}


    pen.setColor(QColor(0,0,255,255));
    painter.fillRect(rectangle1, QColor(0,255,0,255));
    painter.fillRect(rectangle2, QColor(0,255,100,255));
    painter.setPen(pen);
    painter.drawEllipse(rectangle1);
    painter.drawEllipse(rectangle2);

    //painter.end();

}
