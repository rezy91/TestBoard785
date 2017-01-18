#ifndef GRAPMAIN_H
#define GRAPMAIN_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>
#include <QString>

#include <QTime>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QLabel>

#include "common.h"

class Grapmain : public QMainWindow
{
    Q_OBJECT
public:

    struct strHistory
    {
        QList<double> value;
        QList<QTime> time;
    };
    struct strRangeToDisplay
    {
        int indexStart = 0;
        int indexStop = 0;
    };

    explicit Grapmain(QWidget *parent = 0);

    bool WasChangedStateSignal(int source, int stateSignal);
    int GetMinimalResolution(int activeSource[nmbCurvesInGraph], int *sourceResol);
    void startShowGraph(QTime time);
    void findRangesInLog(void);

    QTime findMinTime(void);
    QTime findMaxTime(void);

private:
    //common variables
    const int constPixels = 10;
    const int constSamples = 10;
    const int constVolumePoint = 3;

    const int constBottomLimit = 50;
    const int constTopLimit = 50;
    const int constLeftLimit = 150;
    const int constRightLimit = 170;

    const int constDistanceHorizontalLines_pxs = 50;

    const QString buttonOn = "Stop";
    const QString buttonOff = "Start";


    int msPerPixelValue = 20;

    int timeAppRuns_ms;
    int mThMoving;

    bool mFromStaticToDynamic = false;
    Qt::GlobalColor colorSignal[nmbCurvesInGraph] = {Qt::blue, Qt::cyan, Qt::red, Qt::magenta};

    QList<int> mTimeHistory;


    QScrollBar* scBar = new QScrollBar(Qt::Horizontal, this);
    QPushButton* startStopDisplay = new QPushButton(this);
    QPushButton* changeResolutionUp = new QPushButton(this);
    QPushButton* changeResolutionDown = new QPushButton(this);
    QLabel* resolutionValue = new QLabel(this);

    QTime timeStartLog;
    QTime timeCurrent;//the newest

    int srcDataStream;


    int usedWidth;
    int usedHeight;
    int currentHeight;
    int currentWidth;
    int nmbHorizLines;


    //variables for separate signal
    strHistory mSignalHistory[nmbCurvesInGraph];
    strRangeToDisplay indexToDisplay[nmbCurvesInGraph];

    QString mLegendItems[nmbCurvesInGraph];
    double mMaxCoefficient[nmbCurvesInGraph] = {1, 1, 1, 1};
    int mRefreshTime_ms[nmbCurvesInGraph] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};

    int flagSignalRecord[nmbCurvesInGraph] = {0, 0, 0, 0};


public slots:
    void refreshGraph(QTime currTime, double ssignal, int recStat, QString signalText, int sourceSig, int sourceStream, int flags);
    void refreshCoeffSignal(double coefficient, int source);

protected:
    void paintEvent(QPaintEvent*);

};

#endif // GRAPMAIN_H
