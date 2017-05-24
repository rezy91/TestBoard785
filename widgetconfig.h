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

class widgetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit widgetConfig(QWidget *parent = 0);

signals:

public slots:

private:
    enum {E_NMB_GEN_ADC1 = 9};
    enum {E_NMB_GEN_ADC2 = 7};
    enum {E_NMB_GEN_ADC3 = 4};
    enum {E_NMB_GEN_OTHERS = 9};
    enum {E_NMB_GEN_REGULATOR = 4};
    enum {E_NMB_GEN_TESTTHERAPY = 2};

    enum {E_NMB_AMP_ADC1 = 8};
    enum {E_NMB_AMP_ADC3 = 8};

    enum GEN_ADC_X
    {
        E_GEN_ADC_1,
        E_GEN_ADC_2,
        E_GEN_ADC_3,
        E_GEN_ADC_OTH,
        E_GEN_ADC_NMB
    };

    enum AMP_ADC_X
    {
        E_AMP_ADC_1,
        E_AMP_ADC_3,
        E_AMP_ADC_NMB
    };


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
    QPushButton* buttSendAmpMulAdcx[E_AMP_ADC_NMB];
    QPushButton* buttSendAmpAddAdcx[E_AMP_ADC_NMB];


    QLabel *createNewLabel(const QString &text);
    QGroupBox *createConfigurationGroup();
    QGroupBox *createSettingsGroup();
    QGroupBox *createConfigAmpGroup();
    QGroupBox *createConfigGenGroup();
    QGroupBox *createSettingsAmpGroup();
    QGroupBox *createSettingsGenGroup();

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetConfig_H
