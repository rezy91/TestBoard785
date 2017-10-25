#include "widgetsettings.h"
#include "common.h"

widgetSettings::widgetSettings(QWidget *parent) : QWidget(parent)
{
    MainLayout->addWidget(createSettingsAmpGroup());
    MainLayout->addWidget(createSettingsGenGroup());

    connect(buttSendAmpFreq,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(ACC_AMPLIFIER, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_SET_TYPE_AMPLF_FREQ, 16).rightJustified(2, '0');
        strCmd += QString::number(c_dwVolumeOfPartAmplifier[E_SET_TYPE_AMPLF_FREQ], 16).rightJustified(2, '0');
        strCmd += QString::number(lineInputAmpFreq->text().toInt(), 16).rightJustified(3 * 2, '0');

        emit SendV200specific(strCmd, false);
    });

    connect(buttSendAmpPwm,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(ACC_AMPLIFIER, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_SET_TYPE_AMPLF_PWM, 16).rightJustified(2, '0');
        strCmd += QString::number(c_dwVolumeOfPartAmplifier[E_SET_TYPE_AMPLF_PWM], 16).rightJustified(2, '0');
        uint16_t w_ModifiedParameter = uint16_t((std::numeric_limits<uint16_t>::max() * lineInputAmpPwm->text().toDouble()) / 100);
        strCmd += QString::number(w_ModifiedParameter, 16).rightJustified(2 * 2, '0');

        emit SendV200specific(strCmd, false);
    });


    connect(buttSendGenPwmCool,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_SET_TYPE_RF_COOL_PWM, 16).rightJustified(2, '0');
        strCmd += QString::number(lineInputGenPwmCool->text().toInt(), 16).rightJustified(1 * 2, '0');

        emit SendV200specific(strCmd, false);
    });

    connect(buttSendGenPwrReset,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);
        QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_SET_TYPE_RF_DO_PEAK, 16).rightJustified(2, '0');

        QString strHexNumber = QString::number(lineInputGenPwrReset->text().toInt(), 16);
        strCmd += strHexNumber.rightJustified(1 * 2, '0');

        emit SendV200specific(strCmd, false);
    });

    for(int iDevice = 0; iDevice < E_NMB_AMP_OUTPUTS; iDevice++)
    {
        connect(checkAmpOutput[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);

            QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
            strCmd += QString::number(ACC_AMPLIFIER, 16).rightJustified(2, '0');
            strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
            strCmd += QString::number(E_SET_TYPE_AMPLF_OUTPUTS, 16).rightJustified(2, '0');
            strCmd += QString::number(c_dwVolumeOfPartAmplifier[E_SET_TYPE_AMPLF_OUTPUTS], 16).rightJustified(2, '0');

            for(int jDevice = 0; jDevice < E_NMB_AMP_OUTPUTS; jDevice++)
            {
                strCmd += (checkAmpOutput[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd, false);
        });
    }

    for(int iDevice = 0; iDevice < E_NMB_GEN_OUTPUTS; iDevice++)
    {
        connect(checkGenOutputs[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);

            QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
            strCmd += QString::number(255, 16).rightJustified(2, '0');
            strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
            strCmd += QString::number(E_SET_TYPE_RF_OUTPUTS, 16).rightJustified(2, '0');

            for(int jDevice = 0; jDevice < E_NMB_GEN_OUTPUTS; jDevice++)
            {
                strCmd += (checkGenOutputs[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd, false);
        });
    }

    for(int iDevice = 0; iDevice < E_NMB_GEN_APLx; iDevice++)
    {
        connect(checkGenAplX[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
            strCmd += QString::number(255, 16).rightJustified(2, '0');
            strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
            strCmd += QString::number(E_SET_TYPE_RF_APLS, 16).rightJustified(2, '0');

            for(int jDevice = 0; jDevice < E_NMB_GEN_APLx; jDevice++)
            {
                strCmd += (checkGenAplX[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd, false);

        });
    }


    connect(checkGenRawData[0], &QCheckBox::stateChanged, [=](int checked){

        Q_UNUSED(checked);
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_ADCS_RAW_DATA, 16));

        if(checkGenRawData[0]->checkState() == Qt::Checked)
        {
            strCmd += "010000";
            checkGenRawData[1]->setEnabled(false);
            checkGenRawData[2]->setEnabled(false);
        }
        else
        {
            strCmd += "000000";
            checkGenRawData[1]->setEnabled(true);
            checkGenRawData[2]->setEnabled(true);
        }

        emit SendV200specific(strCmd, false);
    });

    connect(checkGenRawData[1], &QCheckBox::stateChanged, [=](int checked){

        Q_UNUSED(checked);
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_ADCS_RAW_DATA, 16));

        if(checkGenRawData[1]->checkState() == Qt::Checked)
        {
            strCmd += "000100";
            checkGenRawData[0]->setEnabled(false);
            checkGenRawData[2]->setEnabled(false);
        }
        else
        {
            strCmd += "000000";
            checkGenRawData[0]->setEnabled(true);
            checkGenRawData[2]->setEnabled(true);
        }

        emit SendV200specific(strCmd, false);
    });

    connect(checkGenRawData[2], &QCheckBox::stateChanged, [=](int checked){

        Q_UNUSED(checked);
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_ADCS_RAW_DATA, 16));

        if(checkGenRawData[2]->checkState() == Qt::Checked)
        {
            strCmd += "000001";
            checkGenRawData[1]->setEnabled(false);
            checkGenRawData[0]->setEnabled(false);
        }
        else
        {
            strCmd += "000000";
            checkGenRawData[1]->setEnabled(true);
            checkGenRawData[0]->setEnabled(true);
        }

        emit SendV200specific(strCmd, false);
    });

    connect(checkGenTestTherapy, &QPushButton::clicked, [=](){

        QString strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_SET_TYPE_RF_TEST_THERAPY, 16).rightJustified(2, '0');
        strCmd += (checkGenTestTherapy->checkState() == Qt::Checked ? "01" : "00");

        emit SendV200specific(strCmd, false);
    });
}

void widgetSettings::ReadSettingsGener(QByteArray data)
{
    uint8_t byDevice = uint8_t(data.at(1));
    uint8_t byDirection = uint8_t(data.at(2));
    uint8_t byType = uint8_t(data.at(3));

    if(byDevice == 255)
    {
        if(byDirection == ACTION_READ)
        {
            if(byType == E_SET_TYPE_RF_OUTPUTS)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_OUTPUTS; iLoop++)
                {
                    checkGenOutputs[iLoop]->blockSignals(true);
                    checkGenOutputs[iLoop]->setChecked(uint8_t(data.at(5 + iLoop)) == 1 ? true : false);
                    checkGenOutputs[iLoop]->blockSignals(false);
                }
            }
            else if(byType == E_SET_TYPE_RF_APLS)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_APLx; iLoop++)
                {
                    checkGenAplX[iLoop]->blockSignals(true);
                    checkGenAplX[iLoop]->setChecked(uint8_t(data.at(5 + iLoop)) == 1 ? true : false);
                    checkGenAplX[iLoop]->blockSignals(false);
                }
            }
            else if(byType == E_SET_TYPE_RF_COOL_PWM)
            {
                lineInputGenPwmCool->setText(QString::number(uint8_t(data.at(5))));
            }
            else if(byType == E_SET_TYPE_RF_TEST_THERAPY)
            {

            }
            else if(byType == E_SET_TYPE_RF_DO_PEAK)
            {
                lineInputGenPwrReset->setText(QString::number(uint8_t(data.at(5))));
            }
        }
    }
    else if(byDevice == ACC_AMPLIFIER)
    {
        if(byDirection == ACTION_READ)
        {
            qDebug() << data;
        }
    }
}

void widgetSettings::RefreshPage()
{
    for(int iLoop = 0; iLoop < E_SET_TYPE_RF_COUNT; iLoop++)
    {
        QString strCmd;
        strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_READ, 16).rightJustified(2, '0');
        strCmd += QString::number(iLoop, 16).rightJustified(2, '0');
        emit SendV200specific(strCmd, true);
    }

    for(int iLoop = 0; iLoop < E_SET_TYPE_AMPLF_COUNT; iLoop++)
    {
        QString strCmd;
        strCmd = QString("%1").arg(QString::number(PID_SETTINGS_DEVICE, 16));
        strCmd += QString::number(ACC_AMPLIFIER, 16).rightJustified(2, '0');
        strCmd += QString::number(ACTION_READ, 16).rightJustified(2, '0');
        strCmd += QString::number(iLoop, 16).rightJustified(2, '0');
        strCmd += QString::number(c_dwVolumeOfPartAmplifier[iLoop], 16).rightJustified(2, '0');
        emit SendV200specific(strCmd, true);
    }
}

QLabel *widgetSettings::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    //newLabel->setMinimumHeight(10);
    //newLabel->setStyleSheet("border: 1px solid red");
    return newLabel;
}

QGroupBox *widgetSettings::createSettingsAmpGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Amplifier"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffsetColumn = 0;
    int dw_LengthLineEdit = 50;


    //Frequency
    childLayout->addWidget(createNewLabel("Frequency (resolution is 100 Hz)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputAmpFreq = new QLineEdit();
    lineInputAmpFreq->setMaximumWidth(dw_LengthLineEdit);
    buttSendAmpFreq = new QPushButton("Send");

    childLayout->addWidget(lineInputAmpFreq, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendAmpFreq, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Pwm
    childLayout->addWidget(createNewLabel("Pwm (from 0.0 to 100.0 %)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputAmpPwm = new QLineEdit();
    lineInputAmpPwm->setMaximumWidth(dw_LengthLineEdit);
    buttSendAmpPwm = new QPushButton("Send");

    childLayout->addWidget(lineInputAmpPwm, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendAmpPwm, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Outputs
    for(int iLoop = 0; iLoop < E_NMB_AMP_OUTPUTS; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameAmpOutputs[iLoop]), 0, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkAmpOutput[iLoop] = new QCheckBox();
        checkAmpOutput[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkAmpOutput[iLoop], 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }

    groupBox->setLayout(childLayout);

    return groupBox;
}

QGroupBox *widgetSettings::createSettingsGenGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Generator"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffsetColumn = 0;
    int dw_GridOffsetRow = 0;
    int dw_LengthLineEdit = 50;


    //Pwm
    childLayout->addWidget(createNewLabel("Pwm cooling (0 - 100 %)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputGenPwmCool = new QLineEdit();
    lineInputGenPwmCool->setMaximumWidth(dw_LengthLineEdit);
    childLayout->addWidget(lineInputGenPwmCool, 1, dw_GridOffsetColumn, Qt::AlignCenter);

    buttSendGenPwmCool = new QPushButton("Send");
    childLayout->addWidget(buttSendGenPwmCool, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Pwr reset
    childLayout->addWidget(createNewLabel("Pwr reset (0 - 100 us)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputGenPwrReset = new QLineEdit();
    lineInputGenPwrReset->setMaximumWidth(dw_LengthLineEdit);
    buttSendGenPwrReset = new QPushButton("Send");

    childLayout->addWidget(lineInputGenPwrReset, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendGenPwrReset, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Raw data
    for(int iLoop = 0; iLoop < E_NMB_GEN_RAW_RECEIVING; iLoop++)
    {
        childLayout->addWidget(createNewLabel(QString("receive raw ADC_%1").arg(QString::number(iLoop + 1))), 0, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkGenRawData[iLoop] = new QCheckBox();
        checkGenRawData[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkGenRawData[iLoop], 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }
    dw_GridOffsetRow += 2;

    //Outputs
    for(int iLoop = 0; iLoop < E_NMB_GEN_OUTPUTS; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenOutputs[iLoop]), dw_GridOffsetRow, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkGenOutputs[iLoop] = new QCheckBox();
        checkGenOutputs[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkGenOutputs[iLoop], dw_GridOffsetRow + 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }
    dw_GridOffsetRow += 2;

    //AplX
    for(int iLoop = 0; iLoop < E_NMB_GEN_APLx; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenAplX[iLoop]), dw_GridOffsetRow, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkGenAplX[iLoop] = new QCheckBox();
        checkGenAplX[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkGenAplX[iLoop], dw_GridOffsetRow + 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }

    //TestTherapy
    childLayout->addWidget(createNewLabel("Test therapy"), dw_GridOffsetRow, dw_GridOffsetColumn + E_NMB_GEN_APLx, Qt::AlignCenter);
    checkGenTestTherapy = new QCheckBox();
    checkGenTestTherapy->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
    childLayout->addWidget(checkGenTestTherapy, dw_GridOffsetRow + 1, dw_GridOffsetColumn + E_NMB_GEN_APLx, Qt::AlignCenter);

    groupBox->setLayout(childLayout);

    return groupBox;
}
