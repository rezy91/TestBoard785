#include "widgettherapy.h"

widgetTherapy::widgetTherapy(QWidget *parent) : QWidget(parent)
{
    PoweranceLabel->setText("Power");
    DutyCycleLabel->setText("Duty cycle");
    FrequencyLabel->setText("Frequency");
}

void widgetTherapy::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    sliderPowerance->setGeometry(width() / 15 * 0, 0, width() / 20, height() / 10 * 7);
    sliderDutyCycle->setGeometry(width() / 15 * 1, 0, width() / 20, height() / 10 * 7);
    sliderFrequency->setGeometry(width() / 15 * 2, 0, width() / 20, height() / 10 * 7);

    PoweranceLabel->setGeometry(width() / 15 * 0, height() / 10 * 5, width() / 15, height() - 30);
    DutyCycleLabel->setGeometry(width() / 15 * 1, height() / 10 * 5, width() / 15, height() - 30);
    FrequencyLabel->setGeometry(width() / 15 * 2, height() / 10 * 5, width() / 15, height() - 30);
}
