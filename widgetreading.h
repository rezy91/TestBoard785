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

private:
    enum{NMB_AMP_MSGS = 4};
    enum{NMB_GEN_MSGS = 6};


    const QString allNamesAmp[NMB_AMP_MSGS] = {"AMP ADC3 adjusted [ms]", "AMP ADC3 average [ms]", "AMP ADC1 adjusted [ms]", "AMP ADC1 average [ms]"};
    const QString allNamesGen[NMB_GEN_MSGS] = {"GEN ADC3 adjusted [ms]", "GEN ADC3 average [ms]", "GEN ADC2 adjusted [ms]", "GEN ADC2 average [ms]", "GEN ADC1 adjusted [ms]", "GEN ADC1 average [ms]"};

    QSize currSize;

    QVBoxLayout* vBox = new QVBoxLayout(this);
    QTextBrowser* textBrowser = new QTextBrowser(this);
    QPushButton* buttClear = new QPushButton("clear log", this);
    QHBoxLayout* hBoxForSpinGen = new QHBoxLayout(this);
    QHBoxLayout* hBoxForSpinAmp = new QHBoxLayout(this);
    QSpinBox* generTimes[NMB_GEN_MSGS];
    QSpinBox* amplfTimes[NMB_AMP_MSGS];

    QGridLayout* qGridLyout = new QGridLayout(this);
    QCheckBox* chBoxAmp[NMB_AMP_MSGS];
    QCheckBox* chBoxGen[NMB_GEN_MSGS];

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetReading_H
