#ifndef WIDGETTHERAPY_H
#define WIDGETTHERAPY_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>

#include "common.h"

class widgetTherapy : public QWidget
{
    Q_OBJECT
public:
    explicit widgetTherapy(QWidget *parent = 0);

private:

    enum TherapyState{
      E_STATE_OFF,
      E_STATE_READY,
      E_STATE_ON
    };

    enum parameters{
      E_POWER,
      E_DUTYCYCLE,
      E_COOLING,
      E_ULTRASOUND,
      E_PARAMS_NMB
    };

    struct paramTherapy{
        QLabel* name;
        QLabel* value;
        QSlider* slider;
    };

    const QString namesParams[E_PARAMS_NMB] = {"Power", "Duty cycle", "Cooling", "Ultrasound"};
    const QString unitParams[E_PARAMS_NMB] = {"[W]", "[%]", "[°C]", "[W/cm^2]"};
    const int maxSlider[E_PARAMS_NMB] = {100, 100, 45, 15};
    const int minSlider[E_PARAMS_NMB] = {0, 20, 15, 0};
    const int stepSlider[E_PARAMS_NMB] = {1, 20, 1, 1};

    paramTherapy therapyParams[E_PARAMS_NMB];

    QPushButton* startButton = new QPushButton("START", this);
    QPushButton* stopButton = new QPushButton("STOP", this);
    QRadioButton* userButton = new QRadioButton("USER", this);
    QComboBox* listOfChannels = new QComboBox(this);

    int dwCurrentChannel;
    int dwMaximalSetPower;

    void TherapyRuns(unsigned char stateTherapy);
    void TherapyDoesnotRun(void);
    void EnableSliders(TherapyState eState);

public:
    void resetValues(void);

signals:
    void SendV200specific(QString msg, bool bExp);
    void ChoosedChannel(int dwChannel);

public slots:
    void ReceiveStatusReg(STATUS_REGISTER eStatusReg);
    void ReceiveLimitSlider(int index, QString text);
    void ReceiveMaxPower(uint16_t sMaxPower);
    void ReceiveDefaultMaxPower(void);

protected:
    void paintEvent(QPaintEvent*e);
};

#endif // WIDGETTHERAPY_H
