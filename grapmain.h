#ifndef GRAPMAIN_H
#define GRAPMAIN_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>


class Grapmain : public QMainWindow
{
    Q_OBJECT
public:
    enum{nmbCurvesInGraph = 2};

    explicit Grapmain(QWidget *parent = 0);

private:
    int totalTime_ms = 0;
    int refreshTime_ms = 1;
    int nThSample = 1;
    bool fromStaticToDynamic = false;

    int timeExpired[50];//for my display is max value equal 18, therefore reserve


    int printValue1;

    bool bEnableDraw = false;
    QList<int> centers[nmbCurvesInGraph];


    const int constBottomLimit = 50;
    const int constTopLimit = 50;
    const int constLeftLimit = 20;
    const int constRightLimit = 20;

    const int constDiffBetweenTwoPoints = 10;
    const int constVertLineXthPoints = 10;

public slots:
    void Refresh(int mResolution_ms, int mSignalOne);

protected:
    void paintEvent(QPaintEvent*);
};

#endif // GRAPMAIN_H
