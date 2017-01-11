#ifndef GRAPMAIN_H
#define GRAPMAIN_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>
#include <QString>


class Grapmain : public QMainWindow
{
    Q_OBJECT
public:
    enum{nmbCurvesInGraph = 4};

    explicit Grapmain(QWidget *parent = 0);

    bool WasTimeReolutionChanged(int mInputValue_ms[nmbCurvesInGraph]);
    int GetMinimalResolution();

private:
    //common variables
    const int constPixels = 10;
    const int constSamples = 10;
    const int constVolumePoint = 5;

    const int constBottomLimit = 50;
    const int constTopLimit = 50;
    const int constLeftLimit = 80;
    const int constRightLimit = 100;

    const int constDistanceHorizontalLines_pxs = 50;

    int bEnableDraw = 0;//this is, because when start app, PaintEvent occurs

    int mMinimalResolution = 1;


    //variables for separate signal
    QList<int> mSignalHistory[nmbCurvesInGraph];
    const QString mLegendItems[nmbCurvesInGraph] = {"Power", "sin", "cos", "log10"};
    double mMaxCoefficient[nmbCurvesInGraph] = {1, 1, 1, 1};



    int mRefreshTime_ms[nmbCurvesInGraph] = {1, 1, 1, 1};
    int mTotalTime_ms = 0;
    int mThSample = 1;
    bool mFromStaticToDynamic = false;
    QList<int> mTimeExpired;
    int mSignalValue;





public slots:
    void refreshGraph(int mResolution_ms[nmbCurvesInGraph], int signal[nmbCurvesInGraph], double coefficient[nmbCurvesInGraph], int source);

protected:
    void paintEvent(QPaintEvent*);

signals:
    void SendUpdateData(double value);

};

#endif // GRAPMAIN_H
