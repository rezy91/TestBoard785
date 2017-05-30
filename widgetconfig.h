#ifndef widgetConfig_H
#define widgetConfig_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

#include "common.h"

class widgetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit widgetConfig(QWidget *parent = 0);

signals:

public slots:
    void ReadAdcData(QString type, QString device, int index, QString data);
    void ReadRegulator(QString data);
    void ReadTestTherapy(QString data);
    void ReadTestCqmFreq(QString data);

private:
    enum {E_NMB_GEN_ADC1 = 9};
    enum {E_NMB_GEN_ADC2 = 7};
    enum {E_NMB_GEN_ADC3 = 4};
    enum {E_NMB_GEN_OTHERS = 9};
    enum {E_NMB_GEN_REGULATOR = 4};
    enum {E_NMB_GEN_TESTTHERAPY = 2};
    enum {E_NMB_GEN_PWM_CQM = 2};

    enum {E_NMB_AMP_ADC1 = 8};
    enum {E_NMB_AMP_ADC3 = 8};

    const QString c_nameGenRegulator[E_NMB_GEN_REGULATOR] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 1000 ms)"};
    const QString c_nameGenTestTherapy[E_NMB_GEN_TESTTHERAPY] = {"Duty factor (20 - 100 %)", "Frequency (25 - 200 Hz)"};
    const QString c_nameGenCQM[E_NMB_GEN_PWM_CQM] = {"ch_1 (1000 - 100000 Hz)", "ch_2 (1000 - 100000 Hz)"};


    QVBoxLayout* MainLayout = new QVBoxLayout(this);

    QLineEdit *lineInputGenMulAdcx[E_GEN_ADC_NMB][E_NMB_GEN_ADC1];
    QLineEdit *lineInputGenAddAdcx[E_GEN_ADC_NMB][E_NMB_GEN_ADC1];
    QLineEdit *lineInputGenRegulator[E_NMB_GEN_REGULATOR];
    QLineEdit *lineInputGenTestTherapy[E_NMB_GEN_TESTTHERAPY];

    QPushButton* buttSendGenMulAdcx[E_GEN_ADC_NMB];
    QPushButton* buttSendGenAddAdcx[E_GEN_ADC_NMB];
    QPushButton* buttSendGenRegulator;
    QPushButton* buttSendGenTestTherapy;

    QLineEdit *lineInputAmpMulAdcx[E_AMP_ADC_NMB][E_NMB_AMP_ADC1];
    QLineEdit *lineInputAmpAddAdcx[E_AMP_ADC_NMB][E_NMB_AMP_ADC3];
    QLineEdit *lineInputGenPwmCqm[E_NMB_GEN_PWM_CQM];
    QPushButton* buttSendAmpMulAdcx[E_AMP_ADC_NMB];
    QPushButton* buttSendAmpAddAdcx[E_AMP_ADC_NMB];
    QPushButton* buttSendGenPwmCqm;


    QLabel *createNewLabel(const QString &text);
    QGroupBox *createConfigAmpGroup();
    QGroupBox *createConfigGenGroup();

signals:
    void SendV200specific(QString msg);
    void SaveAdcData(QString type, QString device, int index, QString data);
    void SaveRegulator(QString data);
    void SaveTestTherapy(QString data);
    void SaveTestCqmFreq(QString data);

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetConfig_H
