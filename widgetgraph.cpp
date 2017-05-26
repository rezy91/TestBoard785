/*#include "widgetgraph.h"
#include <QPainter>

widgetGraph::widgetGraph(QWidget *parent) : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red);
    setAutoFillBackground(true);
    setPalette(pal);
}

void widgetGraph::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
*/

#include "widgetgraph.h"
#include "mainwindow.h"
#include <QtGui>
#include <QtCore>


widgetGraph::widgetGraph(QWidget *parent) : QWidget(parent)
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        maxLimitLevel[iLoop] = new QDoubleSpinBox(this);
        minLimitLevel[iLoop] = new QDoubleSpinBox(this);
        signalsChoosing[iLoop] = new QComboBox(this);

        maxLimitLevel[iLoop]->setMaximum(99999);
        minLimitLevel[iLoop]->setMaximum(99999);

        mHighLevelAxis_y[iLoop] = 100;
        mLowLevelAxis_y[iLoop] = 0;

        maxLimitLevel[iLoop]->setValue(mHighLevelAxis_y[iLoop]);
        minLimitLevel[iLoop]->setValue(mLowLevelAxis_y[iLoop]);
        signalsChoosing[iLoop]->clear();
    }

    installEventFilter(this);

    openLogButton->setText("open log");
    scBar->setMinimum(0);

    startStopDisplay->setCheckable(true);
    startStopDisplay->setChecked(false);
    startStopDisplay->setText(buttonOn);

    setmaxResolution->setText("Max");
    changeResolutionUp->setText("Up");
    changeResolutionDown->setText("Down");
    setminResolution->setText("Min");
    resolutionValue->setText(QString::number(msPerPixelValue));

    connect(openLogButton, &QPushButton::clicked, dynamic_cast<MainWindow *> (this->parentWidget()), &MainWindow::on_openlogButton_clicked);

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        connect(maxLimitLevel[iLoop],static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

            //qDebug() << " max spin box:" << iLoop << "new value:" << nValue;
            adjustCoefficientSingleStep(maxLimitLevel[iLoop], nValue);
            mHighLevelAxis_y[iLoop] = nValue;
            refrGr("new high level");
        });

        connect(minLimitLevel[iLoop],static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

            //qDebug() << " min spin box:" << iLoop << "new value:" << nValue;
            adjustCoefficientSingleStep(minLimitLevel[iLoop], nValue);
            mLowLevelAxis_y[iLoop] = nValue;
            refrGr("new low level");
        });

        connect(signalsChoosing[iLoop],static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){

            emit FlagSignalChoosed(iLoop, nValue);
        });
    }


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

int widgetGraph::findMinAndMaxTimeInLog()
{
    lowAbsolute = findMinTime();
    highAbsolute = findMaxTime();

    int diffTimeInMsec = QTime(lowAbsolute).msecsTo(highAbsolute);

    scBar->setMaximum(diffTimeInMsec);
    scBar->setValue(scBar->maximum());

    return diffTimeInMsec;
}

void widgetGraph::saveNewSampleToBuffer(int index, QTime time, double signal, QString text)
{
    mSignalHistory[index].value.append(signal);
    mSignalHistory[index].time.append(time);
    mLegendItems[index] = text;
}

bool widgetGraph::isNoSignalDisplayed()
{
    bool retValue = false;

    if((!flagSignalRecord[0]) && (!flagSignalRecord[1]) && (!flagSignalRecord[2]) && (!flagSignalRecord[3]))
    {
        retValue = true;
    }

    return retValue;
}

void widgetGraph::findPreciousTime()
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

void widgetGraph::refrGr(QString nameEvent)
{
    Q_UNUSED(nameEvent);
    //qDebug() << "source repaint:" << nameEvent;
    repaint();
}

int widgetGraph::GetMinimalResolution(int activeSource[], int* sourceResol)
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

void widgetGraph::clearAllVariables(void)
{
    clearAllSignalsHistory();
    startStopDisplay->setChecked(false);
}

void widgetGraph::clearSignalHistory(int indexSignal)
{
    mSignalHistory[indexSignal].value.clear();
    mSignalHistory[indexSignal].time.clear();
}

void widgetGraph::clearAllSignalsHistory()
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        clearSignalHistory(iLoop);
    }
    msPerPixelValue = constMinimalReolution * constLowLevelResolution;
    resolutionValue->setText(QString::number(msPerPixelValue));
}

QTime widgetGraph::findMinTime()
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

QTime widgetGraph::findMaxTime()
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

void widgetGraph::refreshGraph(QTime currTime, double ssignal, int recStat, QString signalText, int sourceSig, int sourceStream, int flags)
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
            int nmbSignalsOn = 0;

            for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
            {
                if(flagSignalRecord[iLoop])
                {
                    nmbSignalsOn++;
                }
            }

            if(!(nmbSignalsOn > 1))
            {
                findMinAndMaxTimeInLog();
                findPreciousTime();
            }

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
                (isNoSignalDisplayed() == true) ? clearAllVariables() : clearSignalHistory(sourceSig);
                refrGr("srcDataStream - LOG_STREAM");
            }
        }
    }
}

void widgetGraph::adjustCoefficientSingleStep(QDoubleSpinBox *p_oubleSpinBox, double newValue)
{
    if(abs(newValue) >= 10000)
    {
        p_oubleSpinBox->setSingleStep(1000.0);
    }
    else if(abs(newValue) >= 1000)
    {
        p_oubleSpinBox->setSingleStep(100.0);
    }
    else if(abs(newValue) >= 100)
    {
        p_oubleSpinBox->setSingleStep(10.0);
    }
    else if(abs(newValue) >= 10)
    {
        p_oubleSpinBox->setSingleStep(1.0);
    }
    else if(abs(newValue) >= 1)
    {
        p_oubleSpinBox->setSingleStep(0.1);
    }
    else if(abs(newValue) >= 0.1)
    {
        p_oubleSpinBox->setSingleStep(0.01);
    }
}

void widgetGraph::clearAll()
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        signalsChoosing[iLoop]->clear();
    }
}

void widgetGraph::addDashAll()
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        signalsChoosing[iLoop]->addItem("-");
    }
}

void widgetGraph::addItems(const QStringList &texts)
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        signalsChoosing[iLoop]->addItems(texts);
    }
}

void widgetGraph::setItemData(int index, const QVariant &value, int role)
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        signalsChoosing[iLoop]->setItemData(index, value, role);
    }
}

void widgetGraph::paintEvent(QPaintEvent*)
{
    QPainter painterMain(this);

    setmaxResolution->setGeometry(currentWidth - 40, 0, 40, 20);
    changeResolutionUp->setGeometry(currentWidth - 40, 0 + 25, 40, 20);
    resolutionValue->setGeometry(currentWidth - 40 + 10, 0 + 50, 40, 20);
    changeResolutionDown->setGeometry(currentWidth - 40, 0 + 50 + 25, 40, 20);
    setminResolution->setGeometry(currentWidth - 40, 0 + 50 + 50, 40, 20);
    startStopDisplay->setGeometry(currentWidth - 40, 0 + 50 + 50 + 25, 40, 20);
    scBar->setGeometry(constLeftLimit - 20, currentHeight - constBottomLimit + 25, usedWidth + 40, 20);
    openLogButton->setGeometry(0 , currentHeight - 25 - 25 - 25 - 25, currentWidth - 5, 23);

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        maxLimitLevel[iLoop]->setGeometry((currentWidth / nmbCurvesInGraph) * iLoop, currentHeight - 25 - 25 - 25, currentWidth / nmbCurvesInGraph - 5, 23);
        minLimitLevel[iLoop]->setGeometry((currentWidth / nmbCurvesInGraph) * iLoop, currentHeight - 25 - 25, currentWidth / nmbCurvesInGraph - 5, 23);
        signalsChoosing[iLoop]->setGeometry((currentWidth / nmbCurvesInGraph) * iLoop, currentHeight - 25, currentWidth / nmbCurvesInGraph - 5, 23);
    }

    //qDebug() << "PaintEvent occurs";0

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
            int offsetTop;


            if(iLoop == 0)
            {
                offsetAxis = constLeftLimit - 15;
                offsetTop = 10;
            }
            else if(iLoop == 1)
            {
                offsetAxis = constLeftLimit - 65;
                offsetTop = 20;
            }
            else if(iLoop == 2)
            {
                offsetAxis = currentWidth - constRightLimit + 20;
                offsetTop = 20;
            }
            else if(iLoop == 3)
            {
                offsetAxis = currentWidth - constRightLimit + 70;
                offsetTop = 10;
            }

            painterMain.setPen(colorSignal[iLoop]);
            painterMain.drawLine(QPoint(offsetAxis, currentHeight - constBottomLimit),QPoint(offsetAxis, constTopLimit - offsetTop));
            painterMain.drawLine(QPoint(offsetAxis, constTopLimit - offsetTop),QPoint( - 10 + offsetAxis, constTopLimit - offsetTop + 10));
            painterMain.drawLine(QPoint(offsetAxis, constTopLimit - offsetTop),QPoint(10 + offsetAxis, constTopLimit - offsetTop + 10));
            painterMain.drawText(QPoint( - offsetTop + offsetAxis, constTopLimit - offsetTop - 5), mLegendItems[iLoop]);
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

    if(flagSignalRecord[0] || flagSignalRecord[1] || flagSignalRecord[2] || flagSignalRecord[3])
    {
        painterMain.setPen(QPen(Qt::black));

        //x-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - 10 - 40, currentHeight - constBottomLimit - 10), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawLine(QPoint(currentWidth - 10 - 40, currentHeight - constBottomLimit + 10), QPoint(currentWidth - 40, currentHeight - constBottomLimit));
        painterMain.drawText(QPoint(currentWidth + 5 - 40, currentHeight - constBottomLimit), "t");

        //y-axis
        painterMain.drawLine(QPoint(constLeftLimit, currentHeight - constBottomLimit),QPoint(constLeftLimit, constTopLimit - 10));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 10),QPoint(constLeftLimit - 10, constTopLimit));
        painterMain.drawLine(QPoint(constLeftLimit, constTopLimit - 10),QPoint(constLeftLimit + 10, constTopLimit));


        painterMain.drawText(QPoint(constLeftLimit, currentHeight - constBottomLimit + 20), lowLevel.toString());
        painterMain.drawText(QPoint(currentWidth - constRightLimit, currentHeight - constBottomLimit + 20), highLevel.toString());

        int diffTimeInMsec = QTime(lowLevel).msecsTo(highLevel);

        QTime diffTime(0,0,0);
        diffTime = diffTime.addMSecs(diffTimeInMsec);

        painterMain.drawText(QPoint(currentWidth / 2, currentHeight - constBottomLimit + 20), diffTime.toString());
    }
}

bool widgetGraph::eventFilter(QObject *, QEvent *event)
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
