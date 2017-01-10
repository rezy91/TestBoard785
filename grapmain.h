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

private:
    int totalTime_ms = 0;
    int refreshTime_ms = 1;
    int nThSample = 1;
    bool fromStaticToDynamic = false;

    int timeExpired[50];//for my display is max value equal 18, therefore reserve


    int printValue1;

    int bEnableDraw = 0;
    QList<int> centers[nmbCurvesInGraph];


    const int constBottomLimit = 50;
    const int constTopLimit = 50;
    const int constLeftLimit = 80;
    const int constRightLimit = 100;

    const int constDistanceHorizontalLines_pxs = 100;

    const QString legendItems[nmbCurvesInGraph] = {"Power", "sin", "cos", "log10"};
    double maxCoefficient[nmbCurvesInGraph] = {1,1,1,1};

public slots:
    void refreshGraph(int mResolution_ms[4], int signal[4], double coefficient[4], int source);

protected:
    void paintEvent(QPaintEvent*);

signals:
   void SendUpdateData(double value);

};

#endif // GRAPMAIN_H
