#include "grapmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>


Grapmain::Grapmain(QWidget *parent) : QMainWindow(parent)
{

    scBar->setMinimum(0);

    startStopDisplay->setCheckable(true);
    startStopDisplay->setChecked(false);
    startStopDisplay->setText(buttonOn);

    changeResolutionUp->setText("Up");
    changeResolutionDown->setText("Down");
    resolutionValue->setText(QString::number(msPerPixelValue));

    connect(scBar, &QScrollBar::valueChanged, [this](){

        //qDebug() << "value:" << scBar->value();

        if(srcDataStream == RECEIVE_STREAM)
        {
            if(startStopDisplay->isChecked())
            {
                findMinAndMaxTimeInLog();
                repaint();
            }
        }
        else if(srcDataStream == LOG_STREAM)
        {
            findMinAndMaxTimeInLog();
            repaint();
        }

    });

    connect(startStopDisplay, &QPushButton::toggled, [this](){

        if(startStopDisplay->isChecked())
        {
            startStopDisplay->setText(buttonOff);
            scBar->setGeometry(constLeftLimit - 20, currentHeight - 30, usedWidth + 40, 20);
        }
        else
        {
            startStopDisplay->setText(buttonOn);

            clearAllVariables(QTime::currentTime());
        }

    });

    connect(changeResolutionUp, &QPushButton::clicked, [this](){

        if(QTime(timeStartLog).msecsTo(timeCurrent) > (usedWidth * msPerPixelValue))
        {
            msPerPixelValue *= 1.5;

            resolutionValue->setText(QString::number(msPerPixelValue));

            if(srcDataStream == RECEIVE_STREAM)
            {
                if(startStopDisplay->isChecked())
                {
                    findMinAndMaxTimeInLog();
                    repaint();
                }
            }
            else if(srcDataStream == LOG_STREAM)
            {
                findMinAndMaxTimeInLog();
                repaint();
            }
        }

    });

    connect(changeResolutionDown, &QPushButton::clicked, [this](){

        if(msPerPixelValue > 2)
        {
            msPerPixelValue /= 1.5;
        }

        resolutionValue->setText(QString::number(msPerPixelValue));



        if(srcDataStream == RECEIVE_STREAM)
        {
            if(startStopDisplay->isChecked())
            {
                findMinAndMaxTimeInLog();
                repaint();
            }
        }
        else if(srcDataStream == LOG_STREAM)
        {
           findMinAndMaxTimeInLog();
           repaint();
        }

    });
}

void Grapmain::findMinAndMaxTimeInLog()
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if(flagSignalRecord[iLoop])
        {
            double ratioLast = float(scBar->value()) / float(scBar->maximum());
            int indexLast = mSignalHistory[iLoop].time.count() * ratioLast;

            if(indexLast < 1)
            {
                indexLast = 1;
            }

            indexToDisplay[iLoop].indexStop = indexLast - 1;
            indexToDisplay[iLoop].indexStart = 0;


            //find first index
            for(int jLoop = indexToDisplay[iLoop].indexStop; jLoop >= 0; jLoop--)
            {
                int diffTime = QTime(mSignalHistory[iLoop].time.at(jLoop)).msecsTo(mSignalHistory[iLoop].time.at(indexToDisplay[iLoop].indexStop));

                if(diffTime >= usedWidth * msPerPixelValue)
                {
                    break;
                }
                else
                {
                    indexToDisplay[iLoop].indexStart = jLoop;
                }
            }

            //find last index
            for(int jLoop = indexToDisplay[iLoop].indexStart; jLoop < (mSignalHistory[iLoop].time.count() - 1); jLoop++)
            {
                int diffTime = QTime(mSignalHistory[iLoop].time.at(indexToDisplay[iLoop].indexStart)).msecsTo(mSignalHistory[iLoop].time.at(jLoop));

                if(diffTime >= usedWidth * msPerPixelValue)
                {
                    break;
                }
                else
                {
                    indexToDisplay[iLoop].indexStop = jLoop;
                }
            }

            //qDebug("signal: %d, ratio: %f, count: %d, indexFirst: %d, indexLast: %d", iLoop, ratioLast, mSignalHistory[iLoop].time.count(), indexToDisplay[iLoop].indexStart, indexToDisplay[iLoop].indexStop);
        }
    }
}

void Grapmain::findDiffTimeInLog()
{
    QTime minTimestamp = findMinTime();
    QTime maxTimestamp = findMaxTime();

    int diffTimeInMsec = QTime(minTimestamp).msecsTo(maxTimestamp);

    scBar->setMaximum(diffTimeInMsec);
    scBar->setValue(scBar->maximum());
}

void Grapmain::saveNewSampleToBuffer(int index, QTime time, double signal, QString text)
{
    mSignalHistory[index].value.append(signal);
    mSignalHistory[index].time.append(time);
    mLegendItems[index] = text;
    timeCurrent = time;
}

bool Grapmain::isNoSignalDisplayed()
{
    bool retValue = false;

    if((!flagSignalRecord[0]) && (!flagSignalRecord[1]) && (!flagSignalRecord[2]) && (!flagSignalRecord[3]))
    {
        retValue = true;
        qDebug() << "no signal displayed";
    }

    return retValue;
}

bool Grapmain::WasChangedStateSignal(int source, int stateSignal)
{
    bool retValue = false;

    if(flagSignalRecord[source] != stateSignal)
    {
        flagSignalRecord[source] = stateSignal;

        if(stateSignal == 0)
        {
            clearSignalHistory(source);
        }

        if(stateSignal == 1)
        {
            retValue = true;
        }
    }

    if(isNoSignalDisplayed())
    {
       clearAllVariables(QTime::currentTime());;
    }

    return retValue;
}

int Grapmain::GetMinimalResolution(int activeSource[], int* sourceResol)
{
    int minValue = std::numeric_limits<int>::max();

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if((mRefreshTime_ms[iLoop] < minValue) && activeSource[iLoop])
        {
            minValue = mRefreshTime_ms[iLoop];
            *sourceResol = iLoop;
        }
    }

    return minValue;
}

void Grapmain::clearAllVariables(QTime time)
{
    clearAllSignalsHistory();
    mTimeHistory.clear();
    mFromStaticToDynamic = false;
    timeAppRuns_ms = 0;
    mThMoving = 0;
    timeStartLog = time;

    startStopDisplay->setChecked(false);
}

void Grapmain::clearSignalHistory(int indexSignal)
{
    mSignalHistory[indexSignal].value.clear();
    mSignalHistory[indexSignal].time.clear();
    indexToDisplay[indexSignal].indexStart = 0;
    indexToDisplay[indexSignal].indexStop = 0;
}

void Grapmain::clearAllSignalsHistory()
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        clearSignalHistory(iLoop);
    }
}

QTime Grapmain::findMinTime()
{
    QList<QTime> firstTimestamp;

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if(flagSignalRecord[iLoop])
        {
            firstTimestamp.append(*std::min_element(mSignalHistory[iLoop].time.begin(), mSignalHistory[iLoop].time.end()));
        }
    }

    QTime minTime = *std::min_element(firstTimestamp.begin(), firstTimestamp.end());
    //qDebug() << "minimal time:" << minTime;

    return minTime;
}

QTime Grapmain::findMaxTime()
{
    QList<QTime> lastTimestamp;

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if(flagSignalRecord[iLoop])
        {
            lastTimestamp.append(*std::max_element(mSignalHistory[iLoop].time.begin(), mSignalHistory[iLoop].time.end()));
        }
    }

    QTime maxTime = *std::max_element(lastTimestamp.begin(), lastTimestamp.end());
    //qDebug() << "maximal time:" << maxTime;

    return maxTime;
}

void Grapmain::refreshGraph(QTime currTime, double ssignal, int recStat, QString signalText, int sourceSig, int sourceStream, int flags)
{
    currentHeight = height();
    currentWidth = width();


    usedWidth = currentWidth - constLeftLimit - constRightLimit;
    nmbHorizLines = (currentHeight - constBottomLimit - constTopLimit) / constDistanceHorizontalLines_pxs;
    usedHeight = nmbHorizLines * constDistanceHorizontalLines_pxs;

    srcDataStream = sourceStream;

    if(srcDataStream == RECEIVE_STREAM)
    {
        if(WasChangedStateSignal(sourceSig, recStat))
        {
            qDebug() << "start showing graph";
            clearAllVariables(currTime);
        }

        if(!startStopDisplay->isChecked())
        {
            if(flagSignalRecord[sourceSig])
            {
                saveNewSampleToBuffer(sourceSig, currTime, ssignal, signalText);

                flagSignalRecord[sourceSig] = recStat;
            }

            for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
            {
                indexToDisplay[iLoop].indexStop = mSignalHistory[iLoop].time.count();

                if(indexToDisplay[iLoop].indexStop)
                {
                    for(int jLoop = (indexToDisplay[iLoop].indexStop - 1); jLoop >= 0; jLoop--)
                    {
                        int diffTime = QTime(mSignalHistory[iLoop].time.at(jLoop)).msecsTo(mSignalHistory[iLoop].time.last());

                        if(diffTime >= usedWidth * msPerPixelValue)
                        {
                            break;
                        }
                        else
                        {
                            indexToDisplay[iLoop].indexStart = jLoop;
                        }
                    }
                }
            }

            findDiffTimeInLog();
        }

        repaint();
    }
    else if(srcDataStream == LOG_STREAM)
    {
        qDebug() << flags;

        if(flags == 2)//all history loaded
        {
            scBar->setGeometry(constLeftLimit - 20, currentHeight - 30, usedWidth + 40, 20);

            findDiffTimeInLog();
            findMinAndMaxTimeInLog();
            repaint();
        }
        else if(flags == 1)//prepare for loading history
        {
            timeStartLog = currTime;
        }
        else//loading
        {
            flagSignalRecord[sourceSig] = recStat;

            if(flagSignalRecord[sourceSig])
            {
                saveNewSampleToBuffer(sourceSig, currTime, ssignal, signalText);
            }
            else
            {
                if(isNoSignalDisplayed())
                {
                    clearAllVariables(QTime::currentTime());;
                }
                else
                {
                    clearSignalHistory(sourceSig);
                }
                repaint();
            }
        }
    }
}

void Grapmain::refreshCoeffSignal(double coefficient, int source)
{
    mMaxCoefficient[source] = coefficient;

    repaint();
}

void Grapmain::paintEvent(QPaintEvent*)
{
    QPainter painterMain(this);

    changeResolutionUp->setGeometry(currentWidth - 40, constTopLimit + 25, 40, 20);
    resolutionValue->setGeometry(currentWidth - 40 + 10, constTopLimit + 50, 40, 20);
    changeResolutionDown->setGeometry(currentWidth - 40, constTopLimit + 50 + 25, 40, 20);
    startStopDisplay->setGeometry(currentWidth - 40, constTopLimit + 50 + 50, 40, 20);

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        //Width higher than range and it´s for first one
        if((QTime(timeStartLog).msecsTo(timeCurrent) > (usedWidth * msPerPixelValue)) && !mFromStaticToDynamic)
        {
            mFromStaticToDynamic = true;


        }

        //if there is something to dipslay
        if(indexToDisplay[iLoop].indexStop)
        {
            //adjust Y-axis according window height
            //find max value
            double mHistoryMaxValue = 0.001;
            for(int kLoop = indexToDisplay[iLoop].indexStart; kLoop < indexToDisplay[iLoop].indexStop; kLoop++)
            {
                if(mSignalHistory[iLoop].value.at(kLoop) > mHistoryMaxValue)
                {
                    mHistoryMaxValue = mSignalHistory[iLoop].value.at(kLoop);
                }
            }

            //actualize coefficient according max value
            double actualCoefficient = mHistoryMaxValue / (double)(usedHeight);
            if(actualCoefficient > mMaxCoefficient[iLoop])
            {
                mMaxCoefficient[iLoop] = actualCoefficient;
                //qDebug() << "value with coeff:" << iLoop << " exceed at: " << maxElement << " actual mMaxCoefficient is: " << mMaxCoefficient[iLoop];
            }


            //draw points
            int drawXvalue;

            for(int jLoop = indexToDisplay[iLoop].indexStart; jLoop < indexToDisplay[iLoop].indexStop; jLoop++)
            {
                int drawYvalue;

                if(mSignalHistory[iLoop].value.at(jLoop) < 0)
                {
                    drawYvalue = -1;
                    painterMain.setBrush(Qt::black);
                }
                else
                {
                    painterMain.setBrush(colorSignal[iLoop]);
                    drawYvalue = (int)(mSignalHistory[iLoop].value.at(jLoop) / mMaxCoefficient[iLoop]);
                }

                if(jLoop)
                {
                    drawXvalue = QTime(mSignalHistory[iLoop].time.at(indexToDisplay[iLoop].indexStart)).msecsTo(mSignalHistory[iLoop].time.at(jLoop));
                    drawXvalue /= msPerPixelValue;
                }
                else
                {
                    drawXvalue = 0;
                }
                QPoint cnt = QPoint(constLeftLimit + drawXvalue, currentHeight - constBottomLimit - drawYvalue);

                painterMain.setPen(QPen(Qt::black));
                painterMain.drawEllipse(cnt,constVolumePoint,constVolumePoint);
            }


            //Draw y-axes
            int offsetAxis;
            double resValue = ((double)mSignalHistory[iLoop].value.last() / mMaxCoefficient[iLoop]);

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
                double maxValue = multiplier * (double)mSignalHistory[iLoop].value.last();

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
                    painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QPoint(constLeftLimit + drawXvalue, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop));
                }
            }

            //Draw legend
            painterMain.setPen(QPen(Qt::black));
            painterMain.setBrush(colorSignal[iLoop]);;
            painterMain.drawEllipse(QPoint(currentWidth - 50, (currentHeight / 2) + iLoop * 20 - 5),constVolumePoint,constVolumePoint);
            painterMain.drawText(QPoint(currentWidth - 40, (currentHeight / 2) + iLoop * 20), mLegendItems[iLoop]);
        }
    }

    if(flagSignalRecord[0] || flagSignalRecord[1] || flagSignalRecord[2] || flagSignalRecord[3])
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
    }

    /*//time axis computing
    if(mMinimalResSource == mSourceEvent)
    {
        timeAppRuns_ms += mMinimalResolution;

        if(maxXValue)
        {
            if(!(timeAppRuns_ms % (mMinimalResolution * constSamples)) && (mMinimalResSource == mSourceEvent))
            {
                mTimeHistory.append(timeAppRuns_ms);
                mHistoryTimeStop = mTimeHistory.count();
            }

            if(mFromStaticToDynamic)
            {
                if(++mThMoving == constSamples)
                {
                    mThMoving = 0;
                    mHistoryTimeStart++;
                }

                scBar->setMaximum(timeAppRuns_ms);
                scBar->setValue(scBar->maximum());
            }
        }
    }*/

/*
    //draw hozironzal lines & x(time) axis
    for(int kLoop = mHistoryTimeStart; kLoop < mTimeHistory.count(); kLoop++)
    {
        painterMain.setPen(QPen(Qt::black));

        if(((mTimeHistory.at(kLoop) / 1000) / 60) > 60)//time more than a hour
        {
            clearAllVariables(QTime::currentTime());
        }

        int minutes = (mTimeHistory.at(kLoop) / 1000) / 60;
        int seconds = (mTimeHistory.at(kLoop) / 1000) % 60;
        int milisec = (mTimeHistory.at(kLoop) % 1000) / 100;
        painterMain.drawText(QPoint(constLeftLimit + widthOneVertLine - mThMoving * constSamples + widthOneVertLine * (kLoop - mHistoryTimeStart) - 20, currentHeight - constBottomLimit + 10), QString("%1:%2,%3").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')).arg(milisec));


        painterMain.setPen(QPen(Qt::lightGray));
        painterMain.drawLine(QPoint(constLeftLimit + widthOneVertLine - mThMoving * constSamples + widthOneVertLine * (kLoop - mHistoryTimeStart), currentHeight - constBottomLimit),QPoint(constLeftLimit + widthOneVertLine - mThMoving * constSamples + widthOneVertLine * (kLoop - mHistoryTimeStart), constTopLimit));
    }*/
}
