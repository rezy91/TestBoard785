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

void Grapmain::refreshGraph(int mResolution_ms[], int signal[], double coefficient[], int recStat[], QString signalText[], int source)
{
    bEnableDraw++;

    if(recStat[source] == 0)
    {
        mSignalHistory[source].clear();
    }

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
        mThSample = 1;
        mFromStaticToDynamic = false;
        mTimeExpired.clear();

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

    int widthOneVertLine = constPixels * constSamples;
    int currentHeight = height();
    int currentWidth = width();
    int nmbVerLines = (currentWidth - constLeftLimit - constRightLimit) / widthOneVertLine;
    int usedWidth = nmbVerLines * widthOneVertLine;

    int nmbHorizLines = (currentHeight - constBottomLimit - constTopLimit) / constDistanceHorizontalLines_pxs;
    int usedHeight = nmbHorizLines * constDistanceHorizontalLines_pxs;




    int timeAppRuns_ms = ((mTimerOverallTime.elapsed() - mStartTime_ms) / mMinimalResolution) * mMinimalResolution;


    if(timeAppRuns_ms >= (((usedWidth) / constPixels) * mRefreshTime_ms[2]))
    {
        if(!(timeAppRuns_ms % (mRefreshTime_ms[2] * constSamples)))
        {
            mTimeExpired.clear();
            for(int iLoop = 0; iLoop <= nmbVerLines; iLoop++)
            {
                mTimeExpired.append(timeAppRuns_ms - (nmbVerLines - iLoop) * mRefreshTime_ms[2] * constSamples);
            }
        }
        if(!mFromStaticToDynamic)
        {
            mFromStaticToDynamic = true;
        }
        else
        {
            if(++mThSample == (constSamples + 1))
            {
                mThSample = 1;
            }
        }
    }
    else
    {
        for(int iLoop = 0; iLoop <= nmbVerLines; iLoop++)
        {
            mTimeExpired.append(mRefreshTime_ms[2] * iLoop * constSamples);
        }
        mThSample = 0;
    }

    if(bEnableDraw > 1)
    {
        //x-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - constRightLimit - 10, currentHeight - constBottomLimit - 10), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - constRightLimit - 10, currentHeight - constBottomLimit + 10), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit));
        painterMain.drawText(QPoint(currentWidth - constRightLimit + 5, currentHeight - constBottomLimit + 5), "t[ms]");

        //y-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit),QPoint(constLeftLimit, constTopLimit - 20));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit - 10, constTopLimit - 20 + 10));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit + 10, constTopLimit - 20 + 10));


        painterMain.drawText(QPoint(constLeftLimit - 10, currentHeight - constBottomLimit + 10), QString("%1").arg(timeAppRuns_ms));

        for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
        {
            int center;
            double dRatio = ((double)mMinimalResolution / (double)mRefreshTime_ms[iLoop]);

            if(iLoop == mSourceEvent)
            {
                if(mSignalValue[iLoop] < 0)
                {
                    center = -1;
                }
                else
                {
                    center = mSignalValue[iLoop];
                }

                if(mSignalHistory[iLoop].count())
                {
                    int maxElement = *std::max_element(mSignalHistory[iLoop].begin(), mSignalHistory[iLoop].end());
                    double actualCoefficient = (double)maxElement / (double)(usedHeight);
                    if(actualCoefficient > mMaxCoefficient[iLoop])
                    {
                        mMaxCoefficient[iLoop] = actualCoefficient;
                        //qDebug() << "value with coeff:" << iLoop << " exceed at: " << maxElement << " actual mMaxCoefficient is: " << mMaxCoefficient[iLoop];

                        emit SendUpdateData(mMaxCoefficient[iLoop], iLoop);
                    }
                }

                if(flagSignalRecord[iLoop])
                {
                    if(mSignalHistory[iLoop].count() >= (int)((double)(usedWidth / constPixels) * dRatio))
                    {
                        mSignalHistory[iLoop].removeFirst();
                    }

                    mSignalHistory[iLoop].append(center);
                }
                else
                {
                    if(mSignalHistory[iLoop].count())
                    {
                       mSignalHistory[iLoop].removeFirst();
                    }
                }

            }

            //draw points
            for(int jLoop = 0; jLoop <= mSignalHistory[iLoop].count(); jLoop++)
            {
                if(jLoop < mSignalHistory[iLoop].count())
                {

                    int drawYvalue = (int)((double)mSignalHistory[iLoop].at(jLoop) / mMaxCoefficient[iLoop]);
                    int drawXvalue;

                    if(constPixels * jLoop)
                    {
                       drawXvalue = (int)((double)(constPixels * jLoop) / dRatio);
                       drawXvalue = (int)(((double)constPixels / dRatio) * (double)jLoop);
                    }
                    else
                    {
                        drawXvalue = 0;
                    }
                    QPoint cnt = QPoint(constLeftLimit + constPixels + drawXvalue, currentHeight - constBottomLimit - drawYvalue);

                    painterMain.setPen(QPen(Qt::black));
                    painterMain.setBrush(colorSignal[iLoop]);;
                    painterMain.drawEllipse(cnt,constVolumePoint,constVolumePoint);
                }


                /*//draw vertical lines plus x-axis texts
                if(!(jLoop % constSamples) && iLoop == 2)
                {

                    int xValue = constLeftLimit + constPixels * jLoop - mThSample * constPixels;

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

                        painterMain.setPen(QPen(Qt::black));


                        if(mThSample == constSamples)
                        {
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), QString("%1").arg(mTimeExpired.at((jLoop / constSamples) - 1)));
                        }
                        else
                        {
                            painterMain.drawText(QPoint(xValue - 10, currentHeight - constBottomLimit + 10), QString("%1").arg(mTimeExpired.at(jLoop / constSamples)));
                        }
                    }
                }*/

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
                            painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop));
                        }
                }
            }


            //Draw legend
            painterMain.setPen(QPen(Qt::black));
            painterMain.setBrush(colorSignal[iLoop]);;
            painterMain.drawEllipse(QPoint(currentWidth - 50, (currentHeight / 2) + iLoop * 20 - 5),constVolumePoint,constVolumePoint);
            painterMain.drawText(QPoint(currentWidth - 40, (currentHeight / 2) + iLoop * 20), mLegendItems[iLoop]);
        }
    }
}
