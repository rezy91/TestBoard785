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

    int widthOneVertLine = constDiffBetweenTwoPoints * constVertLineXthPoints;
    int currentHeight = height();
    int currentWidth = 550;
    int nmbVerLines = currentWidth / widthOneVertLine;
    int usedWidth = nmbVerLines * widthOneVertLine;



    //x-axis
    painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
    painterMain.drawLine(QPoint(currentWidth - constRightLimit - 10, currentHeight - constBottomLimit - 10), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
    painterMain.drawLine(QPoint(currentWidth - constRightLimit - 10, currentHeight - constBottomLimit + 10), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));


    //y-axis
    painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit),QPoint(constLeftLimit, constTopLimit));

    painterMain.drawLine(QPoint(constLeftLimit, constTopLimit),QPoint(constLeftLimit - 10, constTopLimit + 10));
    painterMain.drawLine(QPoint(constLeftLimit, constTopLimit),QPoint(constLeftLimit + 10, constTopLimit + 10));


    if(totalTime_ms >= (((usedWidth - widthOneVertLine) / 10) * refreshTime_ms))
    {
        if(!(totalTime_ms % (refreshTime_ms * 10)))
        {
            for(int iLoop = usedWidth / widthOneVertLine; iLoop > 0; iLoop--)
            {
                timeExpired[usedWidth / widthOneVertLine - iLoop] = ((totalTime_ms - refreshTime_ms * iLoop * 10) + refreshTime_ms * 10) / 1000;
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
        for(int iLoop = 0; iLoop < usedWidth / widthOneVertLine; iLoop++)
        {
            timeExpired[iLoop] = (refreshTime_ms * iLoop) / widthOneVertLine;
        }
        nThSample = 0;
    }

    if(bEnableDraw)
    {
        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            int center;

            int offset = 111 + iLoop * 200;
            int yValue = int(offset + sin(PaintCounter[iLoop]) * 111);
            center = yValue;

            if(iLoop == 0)
            {
                center = printValue1;
            }

            if(centers[iLoop].count() >= (usedWidth - widthOneVertLine) / 10)
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
                    QPoint cnt = QPoint(constLeftLimit + 10 + 10 * jLoop, currentHeight - constBottomLimit - centers[iLoop].at(jLoop));
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
                        painterMain.drawLine(QPoint(xValue, currentHeight - constBottomLimit),QPoint(xValue, constTopLimit + 20));

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
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), showInMinAndSec);
                        }
                        else if(minutes >= 10)
                        {
                            QString showInMinAndSec = QString("%1:0%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), showInMinAndSec);
                        }
                        else if(seconds >= 10)
                        {
                            QString showInMinAndSec = QString("0%1:%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), showInMinAndSec);
                        }
                        else
                        {
                            QString showInMinAndSec = QString("0%1:0%2").arg(minutes).arg(seconds);
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), showInMinAndSec);
                        }

                    }
                }

            }
            PaintCounter[iLoop] += 0.1;
        }
    }
}
