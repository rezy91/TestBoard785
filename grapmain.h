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

    explicit Grapmain(QWidget *parent = 0);

    int GetMinimalResolution(int activeSource[nmbCurvesInGraph], int *sourceResol);
    void clearAllVariables(void);
    void clearSignalHistory(int indexSignal);
    void clearAllSignalsHistory(void);
    int findMinAndMaxTimeInLog(void);
    void saveNewSampleToBuffer(int index, QTime time, double signal, QString text);
    bool isNoSignalDisplayed(void);
    void findPreciousTime(void);
    void refrGr(QString nameEvent);

    QTime findMinTime(void);
    QTime findMaxTime(void);

private:
    //common variables
    const int constPixels = 10;
    const int constSamples = 10;
    const int constVolumePoint = 3;

    const int constBottomLimit = 50;
    const int constTopLimit = 75;
    const int constLeftLimit = 150;
    const int constRightLimit = 170;

    const int constDistanceHorizontalLines_pxs = 50;
    const double constMinimalReolution = 1.1;
    const int constLowLevelResolution = 10;

    const QString buttonOn = "Stop";
    const QString buttonOff = "Start";


    int msPerPixelValue = constMinimalReolution * constLowLevelResolution;

    Qt::GlobalColor colorSignal[nmbCurvesInGraph] = {Qt::blue, Qt::darkGreen, Qt::red, Qt::magenta};

    QScrollBar* scBar = new QScrollBar(Qt::Horizontal, this);
    QPushButton* setmaxResolution = new QPushButton(this);
    QPushButton* changeResolutionUp = new QPushButton(this);
    QPushButton* changeResolutionDown = new QPushButton(this);
    QPushButton* setminResolution = new QPushButton(this);
    QPushButton* startStopDisplay = new QPushButton(this);
    QLabel* resolutionValue = new QLabel(this);

    //min & max time of all signals
    QTime lowAbsolute;
    QTime highAbsolute;

    //range for displaying
    QTime lowLevel;
    QTime highLevel;

    int srcDataStream;

    int usedWidth;
    int usedHeight;
    int currentHeight;
    int currentWidth;
    int nmbHorizLines;

    //variables for separate signal
    strHistory mSignalHistory[nmbCurvesInGraph];
    QString mLegendItems[nmbCurvesInGraph];
    double mHighLevelAxis_y[nmbCurvesInGraph];
    double mLowLevelAxis_y[nmbCurvesInGraph];
    int mRefreshTime_ms[nmbCurvesInGraph] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    int flagSignalRecord[nmbCurvesInGraph] = {0, 0, 0, 0};

public slots:
    void refreshGraph(QTime currTime, double ssignal, int recStat, QString signalText, int sourceSig, int sourceStream, int flags);
    void refreshHighLevel(double level, int source);
    void refreshLowLevel(double level, int source);

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);

};

#endif // GRAPMAIN_H
