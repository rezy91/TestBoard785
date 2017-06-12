#include "widgetsettings.h"
#include "common.h"

widgetSettings::widgetSettings(QWidget *parent) : QWidget(parent)
{
    MainLayout->addWidget(createSettingsAmpGroup());
    MainLayout->addWidget(createSettingsGenGroup());

    connect(buttSendAmpFreq,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString SaveData;
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_AMP_SET_FREQUENCY, 16));
        strCmd += QString::number(lineInputAmpFreq->text().toInt(), 16).rightJustified(3 * 2, '0');
        SaveData.append(QString("%1").arg(QString::number(lineInputAmpFreq->text().toInt())) + " ");

        emit SaveAmpFreq(SaveData);
        emit SendV200specific(strCmd);
    });

    connect(buttSendAmpPwm,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString SaveData;
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_AMP_SET_PWM, 16));
        uint16_t w_ModifiedParameter = uint16_t((std::numeric_limits<uint16_t>::max() * lineInputAmpPwm->text().toDouble()) / 100);
        strCmd += QString::number(w_ModifiedParameter, 16).rightJustified(2 * 2, '0');
        SaveData.append(QString("%1").arg(QString::number(lineInputAmpPwm->text().toDouble())) + " ");

        emit SaveAmpPwm(SaveData);
        emit SendV200specific(strCmd);
    });


    connect(buttSendGenPwmCool,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString SaveData;
        QString strCmd = QString("%1").arg(QString::number(PID_SET_PWM_COOLS_DUTY, 16));

        strCmd += QString::number(lineInputGenPwmCool->text().toInt(), 16).rightJustified(1 * 2, '0');
        SaveData.append(QString("%1").arg(QString::number(lineInputGenPwmCool->text().toInt())) + " ");

        emit SaveGenPwm(SaveData);
        emit SendV200specific(strCmd);
    });

    connect(buttSendGenPwrReset,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);
        AssemblyAplxAndPwrResetPacket(true);
    });

    for(int iDevice = 0; iDevice < E_NMB_AMP_OUTPUTS; iDevice++)
    {
        connect(checkAmpOutput[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            QString strCmd = QString("%1").arg(QString::number(PID_SET_AMP_OUTPUTS, 16));

            for(int jDevice = 0; jDevice < E_NMB_AMP_OUTPUTS; jDevice++)
            {
                strCmd += (checkAmpOutput[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd);
        });
    }

    for(int iDevice = 0; iDevice < E_NMB_GEN_OUTPUTS; iDevice++)
    {
        connect(checkGenOutputs[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            QString strCmd = QString("%1").arg(QString::number(PID_SET_OUTPUTS, 16));

            for(int jDevice = 0; jDevice < E_NMB_GEN_OUTPUTS; jDevice++)
            {
                strCmd += (checkGenOutputs[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd);
        });
    }

    for(int iDevice = 0; iDevice < E_NMB_GEN_APLx; iDevice++)
    {
        connect(checkGenAplX[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            AssemblyAplxAndPwrResetPacket(false);
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

        emit SendV200specific(strCmd);
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

        emit SendV200specific(strCmd);
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

        emit SendV200specific(strCmd);
    });

    connect(checkGenTestTherapy, &QPushButton::clicked, [=](){

        QString strCmd = QString("%1").arg(QString::number(PID_START_STOP_TEST_THERAPY, 16));
        strCmd += (checkGenTestTherapy->checkState() == Qt::Checked ? "0001" : "0000");

        emit SendV200specific(strCmd);
    });
}

void widgetSettings::ReadAmpFreq(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == 1)
    {
        lineInputAmpFreq->setText(arrListSaved.at(0));
    }
    else
    {
        lineInputAmpFreq->setText(c_defaultValueAmpFreq);
    }
}

void widgetSettings::ReadAmpPwm(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == 1)
    {
        lineInputAmpPwm->setText(arrListSaved.at(0));
    }
    else
    {
        lineInputAmpPwm->setText(c_defaultValueAmpPwm);
    }
}

void widgetSettings::ReadGenPwm(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == 1)
    {
        lineInputGenPwmCool->setText(arrListSaved.at(0));
    }
    else
    {
        lineInputGenPwmCool->setText(c_defaultValueGenPwm);
    }
}

void widgetSettings::ReadGenPwr(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == 1)
    {
        lineInputGenPwrReset->setText(arrListSaved.at(0));
    }
    else
    {
        lineInputGenPwrReset->setText(c_defaultValueGenPwr);
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


void widgetSettings::AssemblyAplxAndPwrResetPacket(bool source)
{
    QString SaveData;
    QString strCmd = QString("%1").arg(QString::number(PID_SET_APLS, 16));

    SaveData.append(QString("%1").arg(QString::number(lineInputGenPwrReset->text().toInt())) + " ");

    for(int jDevice = 0; jDevice < E_NMB_GEN_APLx; jDevice++)
    {
        strCmd += (checkGenAplX[jDevice]->checkState() == Qt::Checked ? "01" : "00");

        if(jDevice == 0)
        {
            QString strHexNumber;

            if(source)
            {
                strHexNumber = QString::number(lineInputGenPwrReset->text().toInt(), 16);
            }
            else
            {
                strHexNumber = QString::number(0);
            }
            strCmd += strHexNumber.rightJustified(1 * 2, '0');

        }
    }

    emit SaveGenPwr(SaveData);
    emit SendV200specific(strCmd);
}

