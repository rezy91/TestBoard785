#ifndef widgetReading_H
#define widgetReading_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QDoubleSpinBox>

#include "common.h"

class widgetReading : public QWidget
{
    Q_OBJECT
public:
    explicit widgetReading(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:
    void SendV200Requirement(Qt::CheckState newState, int device, int indexMsg);

public slots:
    void showTextLog(QString showText);

private:
    enum{NMB_TIMERS = 6};
    enum{NMB_AMP_MSGS = NMB_TIMERS - 2};
    enum{NMB_GEN_MSGS = NMB_TIMERS};


    const QString allNamesAmp[NMB_AMP_MSGS] = {"AMP ADC3 adjusted [ms]", "AMP ADC3 average [ms]", "AMP ADC1 adjusted [ms]", "AMP ADC1 average [ms]"};
    const QString allNamesGen[NMB_GEN_MSGS] = {"GEN ADC3 adjusted [ms]", "GEN ADC3 average [ms]", "GEN ADC2 adjusted [ms]", "GEN ADC2 average [ms]", "GEN ADC1 adjusted [ms]", "GEN ADC1 average [ms]"};

    QSize currSize;

    QVBoxLayout* vBox = new QVBoxLayout(this);
    QTextBrowser* textBrowser = new QTextBrowser(this);
    QPushButton* buttClear = new QPushButton("clear log", this);
    QHBoxLayout* hBoxForSpinGen = new QHBoxLayout(this);
    QHBoxLayout* hBoxForSpinAmp = new QHBoxLayout(this);
    QDoubleSpinBox* generTimes[NMB_TIMERS];
    QDoubleSpinBox* amplfTimes[NMB_TIMERS];

    QFormLayout* qFrmLyout = new QFormLayout(this);
    QCheckBox* chBoxAmp[NMB_AMP_MSGS];
    QCheckBox* chBoxGen[NMB_GEN_MSGS];

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetReading_H
