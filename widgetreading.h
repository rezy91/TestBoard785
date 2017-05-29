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
#include <QGridLayout>
#include <QCheckBox>
#include <QSpinBox>

#include "common.h"

class widgetReading : public QWidget
{
    Q_OBJECT
public:
    explicit widgetReading(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:
    void SendV200Requirement(Qt::CheckState newState, int device, int indexMsg);
    void SendNewTime(int timeValue, int device, int indexMsg);

public slots:
    void showTextLog(QString showText);
    void ReadTimeRequests(bool device, int index, int value);

private:
    const QString allNamesAmp[NMB_ITEMS_TIMERS_AMPLF] = {"AMP ADC3 adjusted [ms]", "AMP ADC3 average [ms]", "AMP ADC1 adjusted [ms]", "AMP ADC1 average [ms]"};
    const QString allNamesGen[NMB_ITEMS_TIMERS_GENER] = {"GEN ADC3 adjusted [ms]", "GEN ADC3 average [ms]", "GEN ADC2 adjusted [ms]", "GEN ADC2 average [ms]", "GEN ADC1 adjusted [ms]", "GEN ADC1 average [ms]"};

    QSize currSize;

    QVBoxLayout* vBox = new QVBoxLayout(this);
    QTextBrowser* textBrowser = new QTextBrowser(this);
    QPushButton* buttClear = new QPushButton("clear log", this);
    QSpinBox* generTimes[NMB_ITEMS_TIMERS_GENER];
    QSpinBox* amplfTimes[NMB_ITEMS_TIMERS_AMPLF];

    QGridLayout* qGridLyout = new QGridLayout(this);
    QCheckBox* chBoxAmp[NMB_ITEMS_TIMERS_AMPLF];
    QCheckBox* chBoxGen[NMB_ITEMS_TIMERS_GENER];

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetReading_H
