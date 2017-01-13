#ifndef GRAPMAIN_H
#define GRAPMAIN_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>
#include <QString>


#include <QScrollArea>
#include <QScrollBar>

class Grapmain : public QMainWindow
{
    Q_OBJECT
public:
    enum{nmbCurvesInGraph = 4};
    explicit Grapmain(QWidget *parent = 0);

    bool WasTimeReolutionChanged(int mInputValue_ms[nmbCurvesInGraph]);
    bool WasChangedStateAnySignal(int stateSignal [nmbCurvesInGraph]);
    int GetMinimalResolution(int activeSource[nmbCurvesInGraph]);
    void startShowGraph(void);

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

    int mMinimalResolution = std::numeric_limits<int>::max();
    int mMinimalResSource = std::numeric_limits<int>::max();
    int mSourceEvent;
    int timeAppRuns_ms;
    int mThMoving;

    bool mFromStaticToDynamic = false;
    Qt::GlobalColor colorSignal[nmbCurvesInGraph] = {Qt::blue, Qt::cyan, Qt::red, Qt::magenta};

    QList<int> mTimeHistory;


    QScrollBar* scBar = new QScrollBar(Qt::Horizontal, this);

    //variables for separate signal
    QList<double> mSignalHistory[nmbCurvesInGraph];
    QString mLegendItems[nmbCurvesInGraph];
    double mMaxCoefficient[nmbCurvesInGraph] = {1, 1, 1, 1};
    int mRefreshTime_ms[nmbCurvesInGraph] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};


    double mSignalValue[nmbCurvesInGraph];

    int flagSignalRecord[nmbCurvesInGraph] = {0, 0, 0, 0};


public slots:
    void refreshGraph(int mResolution_ms[nmbCurvesInGraph], double signal[nmbCurvesInGraph], double coefficient[nmbCurvesInGraph], int recStat[nmbCurvesInGraph], QString signalText[], int source);

protected:
    void paintEvent(QPaintEvent*);

signals:
    void SendUpdateData(double value, int index);

};

#endif // GRAPMAIN_H
