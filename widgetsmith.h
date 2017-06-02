/*#ifndef widgetSmith_H
#define widgetSmith_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class widgetSmith : public QWidget
{
    Q_OBJECT
public:
    explicit widgetSmith(QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetSmith_H
*/




#ifndef widgetSmith_H
#define widgetSmith_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>
#include <QPushButton>
#include <QSpinBox>
#include <QPainter>

#include "settings.h"
#include "complex_N.h"

class widgetSmith : public QWidget
{
    Q_OBJECT
public:
    explicit widgetSmith(QWidget *parent = 0);

    quint32 GetNmbPoints(void);
    void SetNmbPoints(quint32 value);

public slots:
    void ReceivedNewData(qreal magnitudeCurrAvg, qreal phaseCurrAvg, qreal magnitudeCurr50, qreal phaseCurr50, qreal magnitudeAvg50, qreal phaseAvg50);
    void ReadData(quint32 readedVal);

private:

    enum {NMB_PHASES = 180};

    typedef struct
    {
        float magnitude;
        float phase_rad;
    } COMPLEX_NUMBER_GONIO;

    typedef struct
    {
        float real;
        float imag;
    } COMPLEX_NUMBER_ALGEB;

    QList<QPointF> axis[3];

    QPushButton* clearButton = new QPushButton(this);
    QSpinBox* nmbDisplayedSamples = new QSpinBox(this);

    quint32 currentNmbPoint;

    complex_N reflFinishAlg[NMB_PHASES];

    complex_N CalculateReflectionRatio(complex_N current, complex_N average);

signals:
    void SaveData(const quint32& nPoints);

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // widgetSmith_H
