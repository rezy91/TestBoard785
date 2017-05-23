#ifndef WIDGETTHERAPY_H
#define WIDGETTHERAPY_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QSlider>
#include <QLabel>

class widgetTherapy : public QWidget
{
    Q_OBJECT
public:
    explicit widgetTherapy(QWidget *parent = 0);

private:
    QLabel* PoweranceLabel = new QLabel(this);
    QLabel* DutyCycleLabel = new QLabel(this);
    QLabel* FrequencyLabel = new QLabel(this);

    QSlider* sliderPowerance = new QSlider(this);
    QSlider* sliderDutyCycle = new QSlider(this);
    QSlider* sliderFrequency = new QSlider(this);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent*e);
};

#endif // WIDGETTHERAPY_H
