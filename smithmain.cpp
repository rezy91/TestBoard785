#include "smithmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>

SmithMain::SmithMain(QWidget *parent) : QMainWindow(parent)
{
    setFixedHeight(900);
    setFixedWidth(900);

    setStyleSheet("background-image: url(smith.png)");

    //setAutoFillBackground(false);
}


void SmithMain::paintEvent(QPaintEvent *e)
{
    static int increment = 0;
    static QList<QRectF> rects;

    QPainter painterMain(this);
    //QPainter painterPict(this->image);

    int Max = 250;
    int Min = -250;
    int randNumberX = ((rand()%(Max-Min+1))+Min);
    int randNumberY = ((rand()%(Max-Min+1))+Min);
    QRectF rectangle(450.0 + randNumberX, 450.0 + randNumberY, 10.0, 10.0);
    rects.append(rectangle);

    /*painterMain.fillRect(rectangle, Qt::green);
    painterMain.setPen(QPen(Qt::blue));
    painterMain.drawRect(rectangle);*/


    if(rects.count() >= 100)
    {
        rects.clear();
    }
    else
    {
        for(int iLoop = 0; iLoop < rects.count(); iLoop++)
        {
            painterMain.fillRect(rects.at(iLoop), Qt::green);
            painterMain.setPen(QPen(Qt::blue));
            painterMain.drawRect(rects.at(iLoop));
        }
    }
    //painterMain.drawImage(rectangle,*this->image);

    increment++;
}
