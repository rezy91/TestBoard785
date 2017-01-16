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

class Grapmain : public QMainWindow
{
    Q_OBJECT
public:

    struct struct_history
    {
        QList<double> value;
        QList<QTime> time;
    };

    enum{nmbCurvesInGraph = 4};
    explicit Grapmain(QWidget *parent = 0);

    bool WasTimeReolutionChanged(int mInputValue_ms[nmbCurvesInGraph]);
    bool WasChangedStateAnySignal(int stateSignal [nmbCurvesInGraph]);
    int GetMinimalResolution(int activeSource[nmbCurvesInGraph], int *sourceResol);
    void startShowGraph(QTime time);

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

    const int constMillisecondsperPixel = 10;

    int mMinimalResolution = std::numeric_limits<int>::max();
    int mMinimalResSource = std::numeric_limits<int>::max();
    int mSourceEvent;
    int timeAppRuns_ms;
    int mThMoving;

    bool mFromStaticToDynamic = false;
    Qt::GlobalColor colorSignal[nmbCurvesInGraph] = {Qt::blue, Qt::cyan, Qt::red, Qt::magenta};

    QList<int> mTimeHistory;


    QScrollBar* scBar = new QScrollBar(Qt::Horizontal, this);

    int mHistoryTimeStart = 0;
    int mHistoryTimeStop = 0;

    QTime timeStartLog;


    //variables for separate signal
    struct_history mSignalHistory[4];

    QString mLegendItems[nmbCurvesInGraph];
    double mMaxCoefficient[nmbCurvesInGraph] = {1, 1, 1, 1};
    int mRefreshTime_ms[nmbCurvesInGraph] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};

    double mHistoryMaxValue[nmbCurvesInGraph];

    int mHistoryPointStart[nmbCurvesInGraph] = {0, 0, 0, 0};
    int mHistoryPointStop[nmbCurvesInGraph] = {0, 0, 0, 0};

    int flagSignalRecord[nmbCurvesInGraph] = {0, 0, 0, 0};


public slots:
    void refreshGraph(QTime currTime, int mResolution_ms[nmbCurvesInGraph], double signal[nmbCurvesInGraph], double coefficient[nmbCurvesInGraph], int recStat[nmbCurvesInGraph], QString signalText[], int source);

protected:
    void paintEvent(QPaintEvent*);

signals:
    void SendUpdateData(double value, int index);

};

#endif // GRAPMAIN_H
