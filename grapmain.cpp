#include "grapmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>

Grapmain::Grapmain(QWidget *parent) : QMainWindow(parent)
{
    showMaximized();
}

void Grapmain::Refresh(int mResolution_ms, int mSignalOne)
{
    if(!bEnableDraw)
    {
        bEnableDraw = true;
    }

    if(refreshTime_ms != mResolution_ms)
    {
        refreshTime_ms = mResolution_ms;

        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            centers[iLoop].clear();
        }
        totalTime_ms = 0;
        nThSample = 0;
        fromStaticToDynamic = false;
    }
    totalTime_ms += mResolution_ms;
    printValue1 = mSignalOne / 10000;

    update();
}

void Grapmain::paintEvent(QPaintEvent*)
{
    static double PaintCounter[nmbCurvesInGraph] = {0,0};

    QPainter painterMain(this);

    int atHoundreads = width() / 100;
    int houndr = atHoundreads * 100;



    //x-axis
    painterMain.drawLine(QPoint(constLeftLimit, height() - constBottomLimit), QPoint(width() - constRightLimit, height() - constBottomLimit));
    painterMain.drawLine(QPoint(width() - constRightLimit - 10, height() - constBottomLimit - 10), QPoint(width() - constRightLimit, height() - constBottomLimit));
    painterMain.drawLine(QPoint(width() - constRightLimit - 10, height() - constBottomLimit + 10), QPoint(width() - constRightLimit, height() - constBottomLimit));


    //y-axis
    painterMain.drawLine(QPoint(constLeftLimit, height() - constBottomLimit),QPoint(constLeftLimit, constTopLimit));

    painterMain.drawLine(QPoint(constLeftLimit, constTopLimit),QPoint(constLeftLimit - 10, constTopLimit + 10));
    painterMain.drawLine(QPoint(constLeftLimit, constTopLimit),QPoint(constLeftLimit + 10, constTopLimit + 10));


    if(totalTime_ms >= (((houndr - 100) / 10) * refreshTime_ms))
    {
        if(!(totalTime_ms % (refreshTime_ms * 10)))
        {
            for(int iLoop = houndr / 100; iLoop > 0; iLoop--)
            {
                timeExpired[houndr / 100 - iLoop] = ((totalTime_ms - refreshTime_ms * iLoop * 10) + refreshTime_ms * 10) / 1000;
            }
        }
        if(!fromStaticToDynamic)
        {
            fromStaticToDynamic = true;
        }
        else
        {
            if(++nThSample == (10 + 1))
            {
                nThSample = 1;
            }
        }
    }
    else
    {
        for(int iLoop = 0; iLoop < houndr / 100; iLoop++)
        {
            timeExpired[iLoop] = (refreshTime_ms * iLoop) / 100;
        }
        nThSample = 0;
    }

    if(bEnableDraw)
    {
        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            int center;

            int offset = 100 + iLoop * 200;
            int yValue = int(offset + sin(PaintCounter[iLoop]) * 100);
            center = yValue;

            if(iLoop == 0)
            {
                center = printValue1;
            }

            if(centers[iLoop].count() >= (houndr - 100) / 10)
            {
                centers[iLoop].removeFirst();
            }

            centers[iLoop].append(center);

            for(int jLoop = 0; jLoop <= centers[iLoop].count(); jLoop++)
            {
                if(jLoop < centers[iLoop].count())
                {
                    painterMain.setPen(QPen(Qt::black));
                    if(iLoop == 0)
                    {
                        painterMain.setBrush(QBrush(Qt::blue));
                    }
                    else
                    {
                        painterMain.setBrush(QBrush(Qt::red));
                    }
                    QPoint cnt = QPoint(constLeftLimit + 10 + 10 * jLoop, height() - constBottomLimit - centers[iLoop].at(jLoop));
                    painterMain.drawEllipse(cnt,3,3);
                }

                if(!(jLoop % 10) && iLoop == 0)
                {
                    int xValue = constLeftLimit + 10 * jLoop - nThSample * 10;

                    if(xValue < constLeftLimit)
                    {
                        continue;
                    }
                    else
                    {
                        painterMain.drawLine(QPoint(xValue, height() - constBottomLimit),QPoint(xValue, constTopLimit + 20));

                        int minutes;
                        int seconds;

                        if(nThSample == 10)
                        {
                            minutes = timeExpired[(jLoop / 10) - 1] / 60;
                            seconds = timeExpired[(jLoop / 10) - 1] % 60;
                        }
                        else
                        {
                            minutes = timeExpired[jLoop / 10] / 60;
                            seconds = timeExpired[jLoop / 10] % 60;
                        }

                        if(minutes >= 10 && seconds >= 10)
                        {
                            QString showInMinAndSec = QString("%1:%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, height() - constBottomLimit + 10), showInMinAndSec);
                        }
                        else if(minutes >= 10)
                        {
                            QString showInMinAndSec = QString("%1:0%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, height() - constBottomLimit + 10), showInMinAndSec);
                        }
                        else if(seconds >= 10)
                        {
                            QString showInMinAndSec = QString("0%1:%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, height() - constBottomLimit + 10), showInMinAndSec);
                        }
                        else
                        {
                            QString showInMinAndSec = QString("0%1:0%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, height() - constBottomLimit + 10), showInMinAndSec);
                        }

                    }
                }

            }
            PaintCounter[iLoop] += 0.1;
        }
    }
}
