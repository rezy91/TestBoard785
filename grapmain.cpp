#include "grapmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>


#include <QApplication>

Grapmain::Grapmain(QWidget *parent) : QMainWindow(parent)
{
    //showMaximized();
    resize(760, 580);

    mTimerOverallTime.start();
}

bool Grapmain::WasTimeReolutionChanged(int mInputValue_ms[])
{
    bool retValue = false;

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if(mRefreshTime_ms[iLoop] != mInputValue_ms[iLoop])
        {
            mRefreshTime_ms[iLoop] = mInputValue_ms[iLoop];
            retValue = true;
        }
    }

    return retValue;
}

bool Grapmain::WasChangedStateAnySignal(int stateSignal[])
{
    bool retValue = false;

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if(flagSignalRecord[iLoop] != stateSignal[iLoop])
        {
            flagSignalRecord[iLoop] = stateSignal[iLoop];
            retValue = true;

            if(stateSignal[iLoop] == 0)
            {
                mSignalHistory[iLoop].clear();
            }
        }
    }

    return retValue;
}

int Grapmain::GetMinimalResolution(int activeSource[])
{
    int minValue = std::numeric_limits<int>::max();

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if((mRefreshTime_ms[iLoop] < minValue) && activeSource[iLoop])
        {
            minValue = mRefreshTime_ms[iLoop];
        }
    }

    return minValue;
}

void Grapmain::refreshGraph(int mResolution_ms[], double signal[], double coefficient[], int recStat[], QString signalText[], int source)
{
    bEnableDraw++;

    //qDebug() << "refresh:" << bEnableDraw;

    mSourceEvent = source;

    if(WasTimeReolutionChanged(mResolution_ms) || WasChangedStateAnySignal(recStat))
    {
        if(GetMinimalResolution(recStat) != mMinimalResolution)
        {
            mMinimalResolution = GetMinimalResolution(recStat);
            qDebug() << "new minimal value: " << mMinimalResolution;
        }

        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            mSignalHistory[iLoop].clear();
        }
        mFromStaticToDynamic = false;

        mStartTime_ms = mTimerOverallTime.elapsed();
    }
    mSignalValue[source] = signal[source];
    mMaxCoefficient[source] = coefficient[source];
    flagSignalRecord[source] = recStat[source];
    mLegendItems[source] = signalText[source];

    repaint();
}

void Grapmain::paintEvent(QPaintEvent*)
{
    QPainter painterMain(this);
    int maxXValue = 0;

    int widthOneVertLine = constPixels * constSamples;
    int currentHeight = height();
    int currentWidth = width();
    int nmbVerLines = (currentWidth - constLeftLimit - constRightLimit) / widthOneVertLine;
    int usedWidth = nmbVerLines * widthOneVertLine;

    int nmbHorizLines = (currentHeight - constBottomLimit - constTopLimit) / constDistanceHorizontalLines_pxs;
    int usedHeight = nmbHorizLines * constDistanceHorizontalLines_pxs;


    int timeAppRuns_ms = ((mTimerOverallTime.elapsed() - mStartTime_ms) / mMinimalResolution) * mMinimalResolution;


    if(bEnableDraw > 1)
    {
        //x-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - 10 - 40, currentHeight - constBottomLimit - 10), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - 10 - 40, currentHeight - constBottomLimit + 10), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawText(QPoint(currentWidth + 5 - 40, currentHeight - constBottomLimit), "t[ms]");

        //y-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit),QPoint(constLeftLimit, constTopLimit - 20));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit - 10, constTopLimit - 20 + 10));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit + 10, constTopLimit - 20 + 10));


        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            double dRatio = ((double)mMinimalResolution / (double)mRefreshTime_ms[iLoop]);

            if(iLoop == mSourceEvent)
            {
                if(mSignalHistory[iLoop].count())
                {
                    double maxElement = *std::max_element(mSignalHistory[iLoop].begin(), mSignalHistory[iLoop].end());
                    double actualCoefficient = maxElement / (double)(usedHeight);
                    if(actualCoefficient > mMaxCoefficient[iLoop])
                    {
                        mMaxCoefficient[iLoop] = actualCoefficient;
                        //qDebug() << "value with coeff:" << iLoop << " exceed at: " << maxElement << " actual mMaxCoefficient is: " << mMaxCoefficient[iLoop];

                        emit SendUpdateData(mMaxCoefficient[iLoop], iLoop);
                    }
                }

                if(flagSignalRecord[iLoop])
                {
                    if(mSignalHistory[iLoop].count() > (int)((double)(usedWidth / constPixels) * dRatio))
                    {
                        mSignalHistory[iLoop].removeFirst();
                        mFromStaticToDynamic = true;
                    }

                    mSignalHistory[iLoop].append(mSignalValue[iLoop]);
                }
            }

            //draw points
            int drawXvalue;

            for(int jLoop = 0; jLoop <= mSignalHistory[iLoop].count(); jLoop++)
            {
                if(jLoop < mSignalHistory[iLoop].count())
                {
                    int drawYvalue;

                    if(mSignalHistory[iLoop].at(jLoop) < 0)
                    {
                        drawYvalue = -1;
                        painterMain.setBrush(Qt::black);;
                    }
                    else
                    {
                        painterMain.setBrush(colorSignal[iLoop]);;
                        drawYvalue = (int)(mSignalHistory[iLoop].at(jLoop) / mMaxCoefficient[iLoop]);
                    }

                    if(constPixels * jLoop)
                    {
                        drawXvalue = (int)((double)(constPixels * jLoop) / dRatio);
                        drawXvalue = (int)(((double)constPixels / dRatio) * (double)jLoop);
                        if(drawXvalue > maxXValue)
                        {
                            maxXValue = drawXvalue;
                        }
                    }
                    else
                    {
                        drawXvalue = 0;
                    }
                    QPoint cnt = QPoint(constLeftLimit + constPixels + drawXvalue, currentHeight - constBottomLimit - drawYvalue);

                    painterMain.setPen(QPen(Qt::black));

                    painterMain.drawEllipse(cnt,constVolumePoint,constVolumePoint);
                }

            }

            //Draw y-axes
            if(mSignalHistory[iLoop].count())
            {
                int offsetAxis;
                double resValue = ((double)mSignalHistory[iLoop].last() / mMaxCoefficient[iLoop]);

                if(resValue >= 1)// can not be divided by zero value
                {
                        if(iLoop == 0)
                        {
                            offsetAxis = constLeftLimit - 15;
                        }
                        else if(iLoop == 1)
                        {
                            offsetAxis = constLeftLimit - 65;
                        }
                        else if(iLoop == 2)
                        {
                            offsetAxis = currentWidth - constRightLimit + 20;
                        }
                        else if(iLoop == 3)
                        {
                            offsetAxis = currentWidth - constRightLimit + 70;
                        }



                        painterMain.setPen(colorSignal[iLoop]);
                        painterMain.drawLine(QPoint(offsetAxis, currentHeight - constBottomLimit),QPoint(offsetAxis, constTopLimit - 20));
                        painterMain.drawLine(QPoint(offsetAxis, constTopLimit - 20),QPoint( - 10 + offsetAxis, constTopLimit - 20 + 10));
                        painterMain.drawLine(QPoint(offsetAxis, constTopLimit - 20),QPoint(10 + offsetAxis, constTopLimit - 20 + 10));
                        painterMain.drawText(QPoint( - 5 + offsetAxis, constBottomLimit - 20 - 5), mLegendItems[iLoop]);


                        double multiplier = (double)(usedHeight) / (resValue);
                        double maxValue = multiplier * (double)mSignalHistory[iLoop].last();

                        for(int kLoop = 1; kLoop < (nmbHorizLines + 1); kLoop++)
                        {
                            painterMain.setPen(colorSignal[iLoop]);
                            if(iLoop == 0 || iLoop == 1)
                            {
                                painterMain.drawText(QPoint(offsetAxis - 40, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QString("%1").arg((maxValue /  nmbHorizLines) * kLoop));
                            }
                            else
                            {
                                painterMain.drawText(QPoint(offsetAxis + 5, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QString("%1").arg((maxValue /  nmbHorizLines) * kLoop));
                            }
                            painterMain.setPen(QPen(Qt::lightGray));
                            painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QPoint(constLeftLimit + constPixels + drawXvalue, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop));
                        }
                }

                //Draw legend
                painterMain.setPen(QPen(Qt::black));
                painterMain.setBrush(colorSignal[iLoop]);;
                painterMain.drawEllipse(QPoint(currentWidth - 50, (currentHeight / 2) + iLoop * 20 - 5),constVolumePoint,constVolumePoint);
                painterMain.drawText(QPoint(currentWidth - 40, (currentHeight / 2) + iLoop * 20), mLegendItems[iLoop]);
            }
        }

        if(maxXValue)
        {
            painterMain.setPen(QPen(Qt::lightGray));
            painterMain.drawLine(QPoint(constLeftLimit + constPixels + maxXValue, currentHeight - constBottomLimit),QPoint(constLeftLimit + constPixels + maxXValue, constTopLimit));
            painterMain.setPen(QPen(Qt::black));
            painterMain.drawText(QPoint(constLeftLimit + constPixels + maxXValue - 20, currentHeight - constBottomLimit + 10), QString("%1").arg(timeAppRuns_ms));
        }
    }
}
