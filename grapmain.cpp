#include "grapmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>

Grapmain::Grapmain(QWidget *parent) : QMainWindow(parent)
{
    resize(900,1800);
}

void Grapmain::Refresh(int someValue)
{
    Q_UNUSED(someValue);
    if(!bEnableDraw)
    {
        bEnableDraw = true;
    }
    update();
}

void Grapmain::paintEvent(QPaintEvent*)
{
    static double PaintCounter[nmbCurvesInGraph] = {0,0,0};
    static QList<int> centers[nmbCurvesInGraph];
    QPainter painterMain(this);

    if(bEnableDraw)
    {
        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            int offset = 100 + iLoop*200;
            int yValue = int(offset + sin(PaintCounter[iLoop]) * 100);
            int center = yValue;

            if(PaintCounter[iLoop] >= 2 * M_PI)
                if(centers[iLoop].count() >= 150)
                {
                    centers[iLoop].removeFirst();
                }
            centers[iLoop].append(center);

            for(int jLoop = 0; jLoop < centers[iLoop].count(); jLoop++)
            {
                painterMain.setPen(QPen(Qt::yellow));
                painterMain.setBrush(QBrush(Qt::blue));
                QPoint cnt = QPoint(10 * jLoop,centers[iLoop].at(jLoop));
                painterMain.drawEllipse(cnt,3,3);
            }
            PaintCounter[iLoop] += 0.1;
        }
    }
}
