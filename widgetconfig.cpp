#include "widgetconfig.h"

widgetConfig::widgetConfig(QWidget *parent) : QWidget(parent)
{
    /*
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::green);
    setAutoFillBackground(true);
    setPalette(pal);*/

    MainLayout->addWidget(createConfigurationGroup());
    MainLayout->addWidget(createSettingsGroup());


    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        connect(buttSendGenMulAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            qDebug() << "input:" << lineInputGenMulAdcx[iDevice][0]->text();

            float flNmb = lineInputGenMulAdcx[iDevice][0]->text().toFloat();

            qDebug() << "float:" << flNmb;
        });
    }
}

QLabel *widgetConfig::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    //newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    //newLabel->setStyleSheet("border: 1px solid red");
    return newLabel;
}

QGroupBox *widgetConfig::createConfigurationGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Configuration"));
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(createConfigAmpGroup());
    layout->addWidget(createConfigGenGroup());

    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox *widgetConfig::createSettingsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Settings"));
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(createSettingsAmpGroup());
    layout->addWidget(createSettingsGenGroup());

    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox *widgetConfig::createConfigAmpGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Amplifier"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_LengthLineEdit = 50;



    for(int iDevice = 0; iDevice < E_AMP_ADC_NMB; iDevice++)
    {
        childLayout->addWidget(createNewLabel(QString("AMP_ADC_%1 multiple").arg(QString::number(iDevice + 1))), 3 * iDevice + 1, 0, Qt::AlignCenter);
        childLayout->addWidget(createNewLabel(QString("AMP_ADC_%1 additive").arg(QString::number(iDevice + 1))), 3 * iDevice + 2, 0, Qt::AlignCenter);

        int nmbChannels;

        if(iDevice == E_AMP_ADC_1)
        {
            nmbChannels = E_NMB_AMP_ADC1;
        }
        else if(iDevice == E_AMP_ADC_3)
        {
            nmbChannels = E_NMB_AMP_ADC3;
        }
        else
        {
            qDebug() << "error: unsupported ADC device";
            return groupBox;
        }

        for(int iLoop = 0; iLoop < nmbChannels; iLoop++)
        {
            childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);

            lineInputAmpMulAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputAmpMulAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputAmpMulAdcx[iDevice][iLoop], 3 * iDevice + 1, 1 + iLoop, Qt::AlignCenter);

            lineInputAmpAddAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputAmpAddAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputAmpAddAdcx[iDevice][iLoop], 3 * iDevice + 2, 1 + iLoop, Qt::AlignCenter);
        }

        buttSendAmpMulAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendAmpMulAdcx[iDevice], 3 * iDevice + 1, nmbChannels + 1, Qt::AlignCenter);

        buttSendAmpAddAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendAmpAddAdcx[iDevice], 3 * iDevice + 2, nmbChannels + 1, Qt::AlignCenter);
    }


    groupBox->setLayout(childLayout);


    return groupBox;
}

QGroupBox *widgetConfig::createConfigGenGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Generator"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffset;
    int dw_LengthLineEdit = 50;

    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        if(iDevice == E_GEN_ADC_OTH)
        {
            childLayout->addWidget(createNewLabel("OTHERS multiple"), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel("OTHERS additive"), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }
        else
        {
            childLayout->addWidget(createNewLabel(QString("GEN_ADC_%1 multiple").arg(QString::number(iDevice + 1))), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel(QString("GEN_ADC_%1 additive").arg(QString::number(iDevice + 1))), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }

        int nmbChannels;

        if(iDevice == E_GEN_ADC_1)
        {
            nmbChannels = E_NMB_GEN_ADC1;
        }
        else if(iDevice == E_GEN_ADC_2)
        {
            nmbChannels = E_NMB_GEN_ADC2;
        }
        else if(iDevice == E_GEN_ADC_3)
        {
            nmbChannels = E_NMB_GEN_ADC3;
        }
        else if(iDevice == E_GEN_ADC_OTH)
        {
            nmbChannels = E_NMB_GEN_OTHERS;
        }
        else
        {
            qDebug() << "error: unsupported ADC device";
            return groupBox;
        }

        for(int iLoop = 0; iLoop < nmbChannels; iLoop++)
        {
            childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);

            lineInputGenMulAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputGenMulAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputGenMulAdcx[iDevice][iLoop], 3 * iDevice + 1, 1 + iLoop, Qt::AlignCenter);

            lineInputGenAddAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputGenAddAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputGenAddAdcx[iDevice][iLoop], 3 * iDevice + 2, 1 + iLoop, Qt::AlignCenter);
        }

        buttSendGenMulAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendGenMulAdcx[iDevice], 3 * iDevice + 1, nmbChannels + 1, Qt::AlignCenter);

        buttSendGenAddAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendGenAddAdcx[iDevice], 3 * iDevice + 2, nmbChannels + 1, Qt::AlignCenter);
    }

    dw_GridOffset = E_GEN_ADC_NMB * 3;

    childLayout->addWidget(createNewLabel("Regulator"), dw_GridOffset + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR; iLoop++)
    {
        childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), dw_GridOffset, 1 + iLoop, Qt::AlignCenter);
        lineInputGenRegulator[iLoop] = new QLineEdit();
        lineInputGenRegulator[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenRegulator[iLoop], dw_GridOffset + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenRegulator = new QPushButton("Send");
    childLayout->addWidget(buttSendGenRegulator, dw_GridOffset + 1, E_NMB_GEN_REGULATOR + 1, Qt::AlignCenter);


    dw_GridOffset += 2;

    childLayout->addWidget(createNewLabel("Test Therapy"), dw_GridOffset + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_TESTTHERAPY; iLoop++)
    {
        childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), dw_GridOffset, 1 + iLoop, Qt::AlignCenter);
        lineInputGenTestTherapy[iLoop] = new QLineEdit();
        lineInputGenTestTherapy[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenTestTherapy[iLoop], dw_GridOffset + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenTestTherapy = new QPushButton("Send");
    childLayout->addWidget(buttSendGenTestTherapy, dw_GridOffset + 1, E_NMB_GEN_TESTTHERAPY + 1, Qt::AlignCenter);


    groupBox->setLayout(childLayout);


    return groupBox;
}

QGroupBox *widgetConfig::createSettingsAmpGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Amplifier"));

    return groupBox;
}

QGroupBox *widgetConfig::createSettingsGenGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Generator"));

    return groupBox;
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
