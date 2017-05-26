/*#ifndef widgetGraph_H
#define widgetGraph_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class widgetGraph : public QWidget
{
    Q_OBJECT
public:
    explicit widgetGraph(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}
    void SetQStart(QSize value) {currStart = value;}

signals:

public slots:

private:
    QSize currSize;
    QSize currStart;

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetGraph_H*/





#ifndef widgetGraph_H
#define widgetGraph_H

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

#include <QDoubleSpinBox>
#include <QComboBox>

#include "common.h"

class widgetGraph : public QWidget
{
    Q_OBJECT
public:

    struct strHistory
    {
        QList<double> value;
        QList<QTime> time;
    };

    explicit widgetGraph(QWidget *parent = 0);

    int GetMinimalResolution(int activeSource[nmbCurvesInGraph], int *sourceResol);
    void clearAllVariables(void);
    void clearSignalHistory(int indexSignal);
    void clearAllSignalsHistory(void);
    int findMinAndMaxTimeInLog(void);
    void saveNewSampleToBuffer(int index, QTime time, double signal, QString text);
    bool isNoSignalDisplayed(void);
    void findPreciousTime(void);
    void refrGr(QString nameEvent);


    void adjustCoefficientSingleStep(QDoubleSpinBox* p_oubleSpinBox, double newValue);

    void clearAll(void);
    void addDashAll(void);
    void addItems(const QStringList &texts);
    void setItemData(int index, const QVariant &value, int role = Qt::UserRole);

    QTime findMinTime(void);
    QTime findMaxTime(void);

private:
    //common variables
    const int constPixels = 10;
    const int constSamples = 10;
    const int constVolumePoint = 3;

    const int constBottomLimit = 150;
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

    QPushButton* openLogButton = new QPushButton(this);
    QScrollBar* scBar = new QScrollBar(Qt::Horizontal, this);
    QPushButton* setmaxResolution = new QPushButton(this);
    QPushButton* changeResolutionUp = new QPushButton(this);
    QPushButton* changeResolutionDown = new QPushButton(this);
    QPushButton* setminResolution = new QPushButton(this);
    QPushButton* startStopDisplay = new QPushButton(this);
    QLabel* resolutionValue = new QLabel(this);
    QDoubleSpinBox* maxLimitLevel[nmbCurvesInGraph];
    QDoubleSpinBox* minLimitLevel[nmbCurvesInGraph];
    QComboBox* signalsChoosing[nmbCurvesInGraph];

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

    int dw_LogTime_ms = 0;

signals:
    void FlagSignalChoosed(int signalIndex, int value);

public slots:
    void refreshGraph(QTime currTime, double ssignal, int recStat, QString signalText, int sourceSig, int sourceStream, int flags);
protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);

};

#endif // widgetGraph_H



