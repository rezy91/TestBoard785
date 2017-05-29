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
    QList<QPointF> axis[3];

    QPushButton* clearButton = new QPushButton(this);
    QSpinBox* nmbDisplayedSamples = new QSpinBox(this);

    quint32 currentNmbPoint;

signals:
    void SaveData(const quint32& nPoints);

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // widgetSmith_H
