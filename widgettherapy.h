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

class widgetTherapy : public QWidget
{
    Q_OBJECT
public:
    explicit widgetTherapy(QWidget *parent = 0);

private:
    enum{nmbChannelsAppls = 4};

    enum parameters{
      E_POWER,
      E_DUTYCYCLE,
      E_FREQUENCY,
      E_PARAMS_NMB
    };

    struct paramTherapy{
        QLabel* name;
        QLabel* value;
        QSlider* slider;
    };

    const QString namesParams[E_PARAMS_NMB] = {"Power", "Duty cycle", "Frequency"};
    const QString unitParams[E_PARAMS_NMB] = {"[W]", "[%]", "[Hz]"};
    const int maxSlider[E_PARAMS_NMB] = {100, 100, 200};
    const int minSlider[E_PARAMS_NMB] = {0, 20, 25};
    const int stepSlider[E_PARAMS_NMB] = {1, 20, 5};

    paramTherapy therapyParams[E_PARAMS_NMB];

    QPushButton* startButton = new QPushButton(this);
    QPushButton* stopButton = new QPushButton(this);
    QComboBox* listOfChannels = new QComboBox(this);

signals:
    void SendV200specific(QString msg);

public slots:
    void ReceivePartStatusReg(unsigned char value);

protected:
    void paintEvent(QPaintEvent*e);
};

#endif // WIDGETTHERAPY_H
