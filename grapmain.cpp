#include "grapmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>

Grapmain::Grapmain(QWidget *parent) : QMainWindow(parent)
{
    //showMaximized();
    resize(760, 580);
}

void Grapmain::refreshGraph(int mResolution_ms[], int signal[], double coefficient[], int source)
{
    Q_UNUSED(source);

    bEnableDraw++;

    if(refreshTime_ms != mResolution_ms[2])
    {
        refreshTime_ms = mResolution_ms[2];

        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            centers[iLoop].clear();
        }
        totalTime_ms = 0;
        nThSample = 1;
        fromStaticToDynamic = false;
    }
    totalTime_ms += mResolution_ms[2];
    printValue1 = signal[0];
    maxCoefficient[0] = coefficient[2];

    update();
}

void Grapmain::paintEvent(QPaintEvent*)
{
    static double PaintCounter[nmbCurvesInGraph] = {0,0,0,0};

    QPainter painterMain(this);

    int widthOneVertLine = constPixels * constSamples;
    int currentHeight = height();
    int currentWidth = width();
    int nmbVerLines = (currentWidth - constLeftLimit - constRightLimit) / widthOneVertLine;
    int usedWidth = nmbVerLines * widthOneVertLine;

    int nmbHorizLines = (currentHeight - constBottomLimit - constTopLimit) / constDistanceHorizontalLines_pxs;
    int usedHeight = nmbHorizLines * constDistanceHorizontalLines_pxs;




    //x-axis
    painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
    painterMain.drawLine(QPoint(currentWidth - constRightLimit - 10, currentHeight - constBottomLimit - 10), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
    painterMain.drawLine(QPoint(currentWidth - constRightLimit - 10, currentHeight - constBottomLimit + 10), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
    painterMain.drawText(QPoint(currentWidth - constRightLimit + 5, currentHeight - constBottomLimit + 5), "t[s]");

    //y-axis
    painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit),QPoint(constLeftLimit, constTopLimit - 20));
    painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit - 10, constTopLimit - 20 + 10));
    painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit + 10, constTopLimit - 20 + 10));
    painterMain.drawText(QPoint(constLeftLimit - 5, constBottomLimit - 20 - 5), legendItems[0] + " [mW]");


    if(totalTime_ms >= (((usedWidth) / constPixels) * refreshTime_ms))
    {
        if(!(totalTime_ms % (refreshTime_ms * constSamples)))
        {
            for(int iLoop = 0; iLoop <= nmbVerLines; iLoop++)
            {
                timeExpired[iLoop] = (totalTime_ms -   (nmbVerLines - iLoop) * refreshTime_ms * constSamples);
            }
        }
        if(!fromStaticToDynamic)
        {
            fromStaticToDynamic = true;
        }
        else
        {
            if(++nThSample == (constSamples + 1))
            {
                nThSample = 1;
            }
        }
    }
    else
    {
        for(int iLoop = 0; iLoop <= nmbVerLines; iLoop++)
        {
            timeExpired[iLoop] = (refreshTime_ms * iLoop * constSamples);
        }
        nThSample = 0;
    }

    if(bEnableDraw > 1)
    {
        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            int center;

            if(iLoop == 0)
            {
                if(printValue1 < 0)
                {
                    center = -1;
                }
                else
                {
                    center = printValue1;
                }

                int maxElement = *std::max_element(centers[iLoop].begin(), centers[iLoop].end());
                double actualCoefficient = (double)maxElement / (double)(usedHeight);
                if(actualCoefficient > maxCoefficient[iLoop])
                {
                    maxCoefficient[iLoop] = actualCoefficient;
                    qDebug() << "value exceed at: " << maxElement << " actual maxCoefficient is: " << maxCoefficient[iLoop];

                    emit SendUpdateData(maxCoefficient[iLoop]);
                }

            }
            else if(iLoop == 1)
            {
                center = int(200 + sin(PaintCounter[iLoop]) * 111);
            }
            else if(iLoop == 2)
            {
                center = int(200 + cos(PaintCounter[iLoop]) * 111);
            }
            else if(iLoop == 3)
            {
                center = int(200 + log10(PaintCounter[iLoop]) * 111);
            }

            if(centers[iLoop].count() >= (usedWidth) / constPixels)
            {
                centers[iLoop].removeFirst();
            }

            centers[iLoop].append(center);

            for(int jLoop = 0; jLoop <= centers[iLoop].count(); jLoop++)
            {
                if(jLoop < centers[iLoop].count())
                {
                    painterMain.setPen(QPen(Qt::lightGray));
                    if(iLoop == 0)
                    {
                        painterMain.setBrush(QBrush(Qt::blue));
                    }
                    else if(iLoop == 1)
                    {
                        painterMain.setBrush(QBrush(Qt::red));
                    }
                    else if(iLoop == 2)
                    {
                        painterMain.setBrush(QBrush(Qt::green));
                    }
                    else if(iLoop == 3)
                    {
                        painterMain.setBrush(QBrush(Qt::darkYellow));
                    }

                    int drawYvalue = (int)((double)centers[iLoop].at(jLoop) / maxCoefficient[iLoop]);
                    QPoint cnt = QPoint(constLeftLimit + constPixels + constPixels * jLoop, currentHeight - constBottomLimit - drawYvalue);

                    painterMain.drawEllipse(cnt,constVolumePoint,constVolumePoint);
                }

                if(!(jLoop % constSamples) && iLoop == 0)
                {
                    int xValue = constLeftLimit + constPixels * jLoop - nThSample * constPixels;

                    if(xValue < constLeftLimit)
                    {
                        continue;
                    }
                    else
                    {
                        if(xValue >= constLeftLimit + constPixels)
                        {
                            painterMain.drawLine(QPoint(xValue, currentHeight - constBottomLimit),QPoint(xValue, constTopLimit));
                        }

                        int minutes;
                        int seconds;

                        if(nThSample == constSamples)
                        {
                            minutes = timeExpired[(jLoop / 10) - 1] / 60;
                            seconds = timeExpired[(jLoop / 10) - 1] % 60;
                        }
                        else
                        {
                            minutes = timeExpired[jLoop / 10] / 60;
                            seconds = timeExpired[jLoop / 10] % 60;
                        }
                        painterMain.setPen(QPen(Qt::black));


                        if(nThSample == constSamples)
                        {
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), QString("%1").arg(timeExpired[(jLoop / constSamples) - 1]));
                        }
                        else
                        {
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), QString("%1").arg(timeExpired[jLoop / constSamples]));
                        }

                        /*
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
*/
                    }
                }

            }
            PaintCounter[iLoop] += 0.1;
            if(PaintCounter[iLoop] >= 2 * M_PI)
            {
                PaintCounter[iLoop] = 0;
            }


            painterMain.setPen(QPen(Qt::black));

            //Draw y-axes
            if(iLoop == 0)
            {
                double resValue = ((double)centers[iLoop].last() / maxCoefficient[iLoop]);
                if(resValue >= 1)// can not be divided by zero value
                {
                    double multiplier = (double)(usedHeight) / (resValue);
                    double maxValue = multiplier * (double)centers[iLoop].last();

                    for(int kLoop = 1; kLoop < (nmbHorizLines + 1); kLoop++)
                    {
                        painterMain.setPen(QPen(Qt::black));
                        painterMain.drawText(QPoint(constLeftLimit - 40, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QString("%1").arg((maxValue /  nmbHorizLines) * kLoop));

                        painterMain.setPen(QPen(Qt::lightGray));
                        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop));
                    }
                    painterMain.setPen(QPen(Qt::black));
                }
            }

            //Draw legende
            painterMain.drawEllipse(QPoint(currentWidth - constRightLimit + 30, (currentHeight / 2) + iLoop * 20 - 5),constVolumePoint,constVolumePoint);
            painterMain.drawText(QPoint(currentWidth - constRightLimit + 40, (currentHeight / 2) + iLoop * 20), legendItems[iLoop]);
        }
    }
}
