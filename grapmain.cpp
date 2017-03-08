#include "grapmain.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>


Grapmain::Grapmain(QWidget *parent) : QMainWindow(parent)
{
    installEventFilter(this);

    scBar->setMinimum(0);

    startStopDisplay->setCheckable(true);
    startStopDisplay->setChecked(false);
    startStopDisplay->setText(buttonOn);

    setmaxResolution->setText("Max");
    changeResolutionUp->setText("Up");
    changeResolutionDown->setText("Down");
    setminResolution->setText("Min");
    resolutionValue->setText(QString::number(msPerPixelValue));

    connect(scBar, &QScrollBar::valueChanged, [this](){

        //qDebug() << "value:" << scBar->value();

        if(srcDataStream == RECEIVE_STREAM)
        {
            if(startStopDisplay->isChecked())
            {
                findPreciousTime();
                refrGr("scBar");
            }
        }
        else if(srcDataStream == LOG_STREAM)
        {
            findPreciousTime();
            refrGr("scBar");
        }

    });

    connect(setmaxResolution, &QPushButton::clicked, [this](){

        int resultValue = findMinAndMaxTimeInLog() / usedWidth;

        if(resultValue > constMinimalReolution * constLowLevelResolution)
        {
            msPerPixelValue = resultValue;
            resolutionValue->setText(QString::number(msPerPixelValue));

            findPreciousTime();
            refrGr("setmaxResolution");
        }

    });

    connect(changeResolutionUp, &QPushButton::clicked, [this](){

        if(QTime(lowAbsolute).msecsTo(highAbsolute) > (usedWidth * msPerPixelValue))
        {
            int potentialNewresolution = msPerPixelValue * constMinimalReolution;

            if(QTime(lowAbsolute).msecsTo(highAbsolute) > (usedWidth * potentialNewresolution))
            {
                msPerPixelValue = potentialNewresolution;
                resolutionValue->setText(QString::number(msPerPixelValue));

                if(srcDataStream == RECEIVE_STREAM)
                {
                    if(startStopDisplay->isChecked())
                    {
                        findPreciousTime();
                        refrGr("changeResolutionUp");
                    }
                }
                else if(srcDataStream == LOG_STREAM)
                {
                    findPreciousTime();
                    refrGr("changeResolutionUp");
                }
            }
        }

    });

    connect(changeResolutionDown, &QPushButton::clicked, [this](){

        if(msPerPixelValue > constMinimalReolution * constLowLevelResolution)
        {
            msPerPixelValue /= constMinimalReolution;
            resolutionValue->setText(QString::number(msPerPixelValue));
        }


        if(srcDataStream == RECEIVE_STREAM)
        {
            if(startStopDisplay->isChecked())
            {
                findPreciousTime();
                refrGr("changeResolutionDown");
            }
        }
        else if(srcDataStream == LOG_STREAM)
        {
            findPreciousTime();
            refrGr("changeResolutionDown");
        }

    });

    connect(setminResolution, &QPushButton::clicked, [this](){

        msPerPixelValue = constMinimalReolution * constLowLevelResolution;
        resolutionValue->setText(QString::number(msPerPixelValue));

        findPreciousTime();
        refrGr("setminResolution");

    });

    connect(startStopDisplay, &QPushButton::toggled, [this](){

        if(srcDataStream == RECEIVE_STREAM)
        {
            if(startStopDisplay->isChecked())
            {
                startStopDisplay->setText(buttonOff);
            }
            else
            {
                startStopDisplay->setText(buttonOn);
                clearAllVariables();
            }
        }

    });
}

int Grapmain::findMinAndMaxTimeInLog()
{
    lowAbsolute = findMinTime();
    highAbsolute = findMaxTime();

    int diffTimeInMsec = QTime(lowAbsolute).msecsTo(highAbsolute);

    scBar->setMaximum(diffTimeInMsec);
    scBar->setValue(scBar->maximum());

    return diffTimeInMsec;
}

void Grapmain::saveNewSampleToBuffer(int index, QTime time, double signal, QString text)
{
    mSignalHistory[index].value.append(signal);
    mSignalHistory[index].time.append(time);
    mLegendItems[index] = text;
}

bool Grapmain::isNoSignalDisplayed()
{
    bool retValue = false;

    if((!flagSignalRecord[0]) && (!flagSignalRecord[1]) && (!flagSignalRecord[2]) && (!flagSignalRecord[3]))
    {
        retValue = true;
    }

    return retValue;
}

void Grapmain::findPreciousTime()
{

    int showedWidth = usedWidth * msPerPixelValue;
    int showedHalfWidth = showedWidth / 2;
    int msFromLeft = 0;
    int msFromRight = 0;
    int actualValueBar = scBar->value();

    if(((actualValueBar - showedHalfWidth) >= 0) && ((actualValueBar + showedHalfWidth) <= scBar->maximum()))
    {
        msFromLeft = actualValueBar - showedHalfWidth;
        msFromRight = actualValueBar + showedHalfWidth;

        lowLevel = lowAbsolute.addMSecs(msFromLeft);
        highLevel = lowAbsolute.addMSecs(msFromRight);
    }
    else if((actualValueBar - showedHalfWidth) < 0)
    {
        msFromLeft = 0;
        msFromRight = showedWidth;

        lowLevel = lowAbsolute;
        highLevel = lowAbsolute.addMSecs(showedWidth);
    }
    else if((actualValueBar + showedHalfWidth) > scBar->maximum())
    {
        msFromLeft = scBar->maximum() - showedWidth;
        if(msFromLeft < 0)//in case when is displaying online and all points don´t cover all range of 'showedWidth'
        {
            msFromLeft = 0;
        }

        msFromRight = scBar->maximum();

        lowLevel = lowAbsolute.addMSecs(msFromLeft);
        highLevel = highAbsolute;
    }
    else
    {
        qDebug() << "!!!!!error in computing ranges for displaying signals!!!!!";
    }

    if(lowLevel < lowAbsolute)
    {
        qDebug() << "some bad";
    }

    //qDebug() << lowAbsolute << highAbsolute;
    //qDebug() << "from:" << lowLevel << "to:" << highLevel;
}

void Grapmain::refrGr(QString nameEvent)
{
    Q_UNUSED(nameEvent);
    //qDebug() << "source repaint:" << nameEvent;
    repaint();
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

void Grapmain::clearAllVariables(void)
{
    clearAllSignalsHistory();
    startStopDisplay->setChecked(false);
}

void Grapmain::clearSignalHistory(int indexSignal)
{
    mSignalHistory[indexSignal].value.clear();
    mSignalHistory[indexSignal].time.clear();
}

void Grapmain::clearAllSignalsHistory()
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        clearSignalHistory(iLoop);
    }
    msPerPixelValue = constMinimalReolution * constLowLevelResolution;
    resolutionValue->setText(QString::number(msPerPixelValue));
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
    srcDataStream = sourceStream;

    if(srcDataStream == RECEIVE_STREAM)
    {
        flagSignalRecord[sourceSig] = recStat;

        if(flagSignalRecord[sourceSig] == 0)
        {
            clearSignalHistory(sourceSig);
        }

        if(isNoSignalDisplayed())
        {
           clearAllVariables();
        }

        if(!startStopDisplay->isChecked())
        {
            if(flagSignalRecord[sourceSig])
            {
                saveNewSampleToBuffer(sourceSig, currTime, ssignal, signalText);
                findMinAndMaxTimeInLog();
            }
        }

        findPreciousTime();
        refrGr("srcDataStream - RECEIVE_STREAM");
    }
    else if(srcDataStream == LOG_STREAM)
    {
        if(flags == 1)//all history loaded
        {
            findMinAndMaxTimeInLog();
            findPreciousTime();
            refrGr("srcDataStream - LOG_STREAM");
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
                    clearAllVariables();
                }
                else
                {
                    clearSignalHistory(sourceSig);
                }
                refrGr("srcDataStream - LOG_STREAM");
            }
        }
    }
}

void Grapmain::refreshHighLevel(double level, int source)
{
    mHighLevelAxis_y[source] = level;
    refrGr("new high level");
}

void Grapmain::refreshLowLevel(double level, int source)
{
    mLowLevelAxis_y[source] = level;
    refrGr("new low level");
}

void Grapmain::paintEvent(QPaintEvent*)
{
    QPainter painterMain(this);

    setmaxResolution->setGeometry(currentWidth - 40, constTopLimit, 40, 20);
    changeResolutionUp->setGeometry(currentWidth - 40, constTopLimit + 25, 40, 20);
    resolutionValue->setGeometry(currentWidth - 40 + 10, constTopLimit + 50, 40, 20);
    changeResolutionDown->setGeometry(currentWidth - 40, constTopLimit + 50 + 25, 40, 20);
    setminResolution->setGeometry(currentWidth - 40, constTopLimit + 50 + 50, 40, 20);
    startStopDisplay->setGeometry(currentWidth - 40, constTopLimit + 50 + 50 + 25, 40, 20);
    scBar->setGeometry(constLeftLimit - 20, currentHeight - 30, usedWidth + 40, 20);

    //qDebug() << "PaintEvent occurs";

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        //if there is something to dipslay
        if(mSignalHistory[iLoop].time.count())
        {
            //adjust Y-axis according window height
            //find min and max value
            double mHistoryHighlLevelAxis_y = 0.001;
            double mHistoryLowLevelAxis_y = std::numeric_limits<double>::max();

            for(int kLoop = 0; kLoop < mSignalHistory[iLoop].time.count(); kLoop++)
            {
                if(mSignalHistory[iLoop].time.at(kLoop) >= lowLevel && mSignalHistory[iLoop].time.at(kLoop) <= highLevel)
                {
                    if(mSignalHistory[iLoop].value.at(kLoop) > mHistoryHighlLevelAxis_y)
                    {
                        mHistoryHighlLevelAxis_y = mSignalHistory[iLoop].value.at(kLoop);
                    }
                    if(mSignalHistory[iLoop].value.at(kLoop) < mHistoryLowLevelAxis_y)
                    {
                        mHistoryLowLevelAxis_y = mSignalHistory[iLoop].value.at(kLoop);
                    }
                }
            }

            if(mHistoryLowLevelAxis_y > mLowLevelAxis_y[iLoop])
            {
                mHistoryLowLevelAxis_y = mLowLevelAxis_y[iLoop];
            }


            //actualize coefficient according max value
            double mHistoryCoefficient = (mHistoryHighlLevelAxis_y - mHistoryLowLevelAxis_y) / (double)(usedHeight);

            if(mHistoryHighlLevelAxis_y > mHighLevelAxis_y[iLoop])
            {
                mHighLevelAxis_y[iLoop] = mHistoryHighlLevelAxis_y;
                //qDebug() << "high value:" << iLoop << "actual mHighLevelAxis_y is:" << mHighLevelAxis_y[iLoop];
            }

            if(mHistoryLowLevelAxis_y < mLowLevelAxis_y[iLoop])
            {
                mLowLevelAxis_y[iLoop] = mHistoryLowLevelAxis_y;
                //qDebug() << "low value:" << iLoop << "actual mLowLevelAxis_y is:" << mLowLevelAxis_y[iLoop];
            }

            //draw points
            double updateCoeff = mHistoryCoefficient * ((mHighLevelAxis_y[iLoop] - mLowLevelAxis_y[iLoop]) / (mHistoryHighlLevelAxis_y - mHistoryLowLevelAxis_y));

            for(int jLoop = 0; jLoop < mSignalHistory[iLoop].time.count(); jLoop++)
            {
                if(mSignalHistory[iLoop].time.at(jLoop) >= lowLevel && mSignalHistory[iLoop].time.at(jLoop) <= highLevel)
                {
                    int drawYvalue;
                    int drawXvalue;
                    int tempVar;

                    painterMain.setBrush(colorSignal[iLoop]);
                    drawYvalue = (int)((mSignalHistory[iLoop].value.at(jLoop) - mHistoryLowLevelAxis_y) / updateCoeff);

                    tempVar = QTime(lowLevel).msecsTo(mSignalHistory[iLoop].time.at(jLoop));
                    drawXvalue = tempVar / msPerPixelValue;

                    //qDebug() << tempVar << drawXvalue;
                    QPoint cnt = QPoint(constLeftLimit + drawXvalue, currentHeight - constBottomLimit - drawYvalue);

                    painterMain.setPen(QPen(colorSignal[iLoop]));
                    painterMain.drawEllipse(cnt,constVolumePoint,constVolumePoint);
                }
            }


            //Draw y-axes
            int offsetAxis;
            double maxValue = mHighLevelAxis_y[iLoop] - mHistoryLowLevelAxis_y;

            if(maxValue >= 1)// can not be divided by zero value
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
                painterMain.drawText(QPoint( - 20 + offsetAxis, constTopLimit - 20 - 5), mLegendItems[iLoop]);
                painterMain.drawText(QPoint( - 20 + offsetAxis, constTopLimit - 40 - 5), QString::number(mSignalHistory[iLoop].value.last()));


                for(int kLoop = 0; kLoop < (nmbHorizLines + 1); kLoop++)
                {
                    painterMain.setPen(colorSignal[iLoop]);
                    if(iLoop == 0 || iLoop == 1)
                    {
                        painterMain.drawText(QPoint(offsetAxis - 40, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QString("%1").arg(((maxValue /  nmbHorizLines) * kLoop) + mHistoryLowLevelAxis_y));
                    }
                    else
                    {
                        painterMain.drawText(QPoint(offsetAxis + 5, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QString("%1").arg(((maxValue /  nmbHorizLines) * kLoop) + mHistoryLowLevelAxis_y));
                    }
                    painterMain.setPen(QPen(Qt::lightGray));
                    painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop), QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit - constDistanceHorizontalLines_pxs * kLoop));
                }
            }
        }
    }

    if(flagSignalRecord[0] || flagSignalRecord[1] || flagSignalRecord[2] || flagSignalRecord[3])
    {
        painterMain.setPen(QPen(Qt::black));

        //x-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - 10 - 40, currentHeight - constBottomLimit - 10), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - 10 - 40, currentHeight - constBottomLimit + 10), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawText(QPoint(currentWidth + 5 - 40, currentHeight - constBottomLimit), "t");

        //y-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit),QPoint(constLeftLimit, constTopLimit - 20));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit - 10, constTopLimit - 20 + 10));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 20),QPoint(constLeftLimit + 10, constTopLimit - 20 + 10));


        painterMain.drawText(QPoint(constLeftLimit, currentHeight - constBottomLimit + 20), lowLevel.toString());
        painterMain.drawText(QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit + 20), highLevel.toString());

        int diffTimeInMsec = QTime(lowLevel).msecsTo(highLevel);

        QTime diffTime(0,0,0);
        diffTime = diffTime.addMSecs(diffTimeInMsec);

        painterMain.drawText(QPoint(currentWidth / 2, currentHeight - constBottomLimit + 20), diffTime.toString());
    }

}

bool Grapmain::eventFilter(QObject *, QEvent *event)
{
    bool state = false;

    if(event->type() == QEvent::Resize)
    {
        currentHeight = height();
        currentWidth = width();


        usedWidth = currentWidth - constLeftLimit - constRightLimit;
        nmbHorizLines = (currentHeight - constBottomLimit - constTopLimit) / constDistanceHorizontalLines_pxs;
        usedHeight = nmbHorizLines * constDistanceHorizontalLines_pxs;

        findPreciousTime();
        refrGr("resize widget");
        state = true;
    }
    else
    {
        state = false;

    }
    return state;
}
