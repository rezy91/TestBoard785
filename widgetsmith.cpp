/*#include "widgetsmith.h"
#include <QCoreApplication>

widgetSmith::widgetSmith(QWidget *parent) : QWidget(parent)
{

}

void widgetSmith::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    QString pathToFile = QString("%1/").arg(QCoreApplication::applicationDirPath());
    pathToFile += "smith.png";

    painter.drawPixmap(0, 0, QPixmap(pathToFile).scaled(size()));

    //qDebug() << "Min pathToFile : " << pathToFile;
}*/




#include "widgetsmith.h"
#include "complex_N.h"

#include <QtGui>
#include <QtCore>

widgetSmith::widgetSmith(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);

    clearButton->setText("Clear");
    nmbDisplayedSamples->setMinimum(1);
    nmbDisplayedSamples->setMaximum(9999);





    complex_N o_ReflRatio(0.5, 0, complex_N::GONIO);
    complex_N o_50_ohm(50, 0, complex_N::GONIO);
    complex_N o_referenceImpedance(80, 1.57, complex_N::GONIO);



    COMPLEX_NUMBER_GONIO m_ReflRatioGon;
    m_ReflRatioGon.magnitude = 0.5;

    COMPLEX_NUMBER_GONIO m_50_ohm;
    m_50_ohm.magnitude = 50;
    m_50_ohm.phase_rad = 0;

    COMPLEX_NUMBER_GONIO m_referenceImpedance;
    m_referenceImpedance.magnitude = 80;
    m_referenceImpedance.phase_rad = 1.57;

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        COMPLEX_NUMBER_ALGEB m_ReflRatioAlg;

        COMPLEX_NUMBER_ALGEB m_DividentAlg;
        COMPLEX_NUMBER_GONIO m_DividentGon;
        COMPLEX_NUMBER_ALGEB m_DivisorAlg;
        COMPLEX_NUMBER_GONIO m_DivisorGon;

        COMPLEX_NUMBER_GONIO m_FractionGon;
        COMPLEX_NUMBER_GONIO m_ResultGon;

        m_ReflRatioGon.phase_rad = ((2 * 3.14159265358) / NMB_PHASES) * iLoop;

        m_ReflRatioAlg.real = m_ReflRatioGon.magnitude * cos(m_ReflRatioGon.phase_rad);
        m_ReflRatioAlg.imag = m_ReflRatioGon.magnitude * sin(m_ReflRatioGon.phase_rad);

        m_DividentAlg.real = 1 + m_ReflRatioAlg.real;
        m_DividentAlg.imag = m_ReflRatioAlg.imag;
        m_DivisorAlg.real = 1 - m_ReflRatioAlg.real;
        m_DivisorAlg.imag = - m_ReflRatioAlg.imag;

        m_DividentGon.magnitude = sqrt(m_DividentAlg.real * m_DividentAlg.real + m_DividentAlg.imag * m_DividentAlg.imag);

        if(m_DividentAlg.imag < 0.0f)
        {
            m_DividentGon.phase_rad = - acos(m_DividentAlg.real / m_DividentGon.magnitude);
        }
        else
        {
            m_DividentGon.phase_rad = acos(m_DividentAlg.real / m_DividentGon.magnitude);
        }


        m_DivisorGon.magnitude = sqrt(m_DivisorAlg.real * m_DivisorAlg.real + m_DivisorAlg.imag * m_DivisorAlg.imag);

        if(m_DivisorAlg.imag < 0.0f)
        {
            m_DivisorGon.phase_rad = - acos(m_DivisorAlg.real / m_DivisorGon.magnitude);
        }
        else
        {
            m_DivisorGon.phase_rad = acos(m_DivisorAlg.real / m_DivisorGon.magnitude);
        }


        m_FractionGon.magnitude = m_DividentGon.magnitude / m_DivisorGon.magnitude;
        m_FractionGon.phase_rad = m_DividentGon.phase_rad - m_DivisorGon.phase_rad;



        m_ResultGon.magnitude = m_referenceImpedance.magnitude * m_FractionGon.magnitude;
        m_ResultGon.phase_rad = m_referenceImpedance.phase_rad + m_FractionGon.phase_rad;

        COMPLEX_NUMBER_GONIO reflFinishGon;

        reflFinishGon = CalculateReflectionRatio(m_ResultGon, m_50_ohm);


        reflFinishAlg[iLoop].real = reflFinishGon.magnitude * cos(reflFinishGon.phase_rad);
        reflFinishAlg[iLoop].imag = reflFinishGon.magnitude * sin(reflFinishGon.phase_rad);


        qDebug() << m_ResultGon.magnitude << m_ResultGon.phase_rad;
        qDebug() << reflFinishAlg[iLoop].real << reflFinishAlg[iLoop].imag;

    }




    connect(clearButton, &QPushButton::clicked, [this](){

        for(int jLoop = 0; jLoop < 3; jLoop++)
        {
            axis[jLoop].clear();
        }

        repaint();
    });

    connect(nmbDisplayedSamples, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int newValue){

        for(int jLoop = 0; jLoop < 3; jLoop++)
        {
            if(axis[jLoop].count() > newValue)
            {
                int removeItems = axis[jLoop].count() - newValue;

                for(int iLoop = 0; iLoop < removeItems; iLoop++)
                {
                    axis[jLoop].removeFirst();
                }
            }
        }

        currentNmbPoint = newValue;

        emit SaveData(newValue);
        //qDebug() << "value changed:" << newValue;


    });


}

quint32 widgetSmith::GetNmbPoints()
{
    return nmbDisplayedSamples->value();
}

void widgetSmith::SetNmbPoints(quint32 value)
{
    nmbDisplayedSamples->setValue(value);
    currentNmbPoint = value;
}

void widgetSmith::ReceivedNewData(qreal magnitudeCurrAvg, qreal phaseCurrAvg, qreal magnitudeCurr50, qreal phaseCurr50, qreal magnitudeAvg50, qreal phaseAvg50)
{
    axis[0].append(QPointF(magnitudeAvg50, phaseAvg50));
    axis[1].append(QPointF(magnitudeCurr50, phaseCurr50));
    axis[2].append(QPointF(magnitudeCurrAvg, phaseCurrAvg));

    repaint();
}

void widgetSmith::ReadData(quint32 readedVal)
{
    if(readedVal)
    {
        nmbDisplayedSamples->setValue(readedVal);
    }
    else
    {
        nmbDisplayedSamples->setValue(100);
    }
}

widgetSmith::COMPLEX_NUMBER_GONIO widgetSmith::CalculateReflectionRatio(widgetSmith::COMPLEX_NUMBER_GONIO current, widgetSmith::COMPLEX_NUMBER_GONIO average)
{
    COMPLEX_NUMBER_GONIO ReflectionRatio;

    float averageAlgebReal = average.magnitude * cos(average.phase_rad);
    float averageAlgebImag = average.magnitude * sin(average.phase_rad);

    float recentAlgebReal = current.magnitude * cos(current.phase_rad);
    float recentAlgebImag = current.magnitude * sin(current.phase_rad);

    float dividentAlgebReal = recentAlgebReal - averageAlgebReal;
    float dividentAlgebImag = recentAlgebImag - averageAlgebImag;

    float divisorAlgebReal = recentAlgebReal + averageAlgebReal;
    float divisorAlgebImag = recentAlgebImag + averageAlgebImag;


    float commonDivisor = divisorAlgebReal * divisorAlgebReal + divisorAlgebImag * divisorAlgebImag;
    float realPart = (dividentAlgebReal * divisorAlgebReal + dividentAlgebImag * divisorAlgebImag) / commonDivisor;
    float imagPart = (dividentAlgebImag * divisorAlgebReal - dividentAlgebReal * divisorAlgebImag) / commonDivisor;

    ReflectionRatio.magnitude = sqrt(realPart * realPart + imagPart * imagPart);
    ReflectionRatio.phase_rad = acos(realPart / ReflectionRatio.magnitude);

    if(imagPart < 0.0f)
    {
        ReflectionRatio.phase_rad = - acos(realPart / ReflectionRatio.magnitude);
    }
    else
    {
        ReflectionRatio.phase_rad = acos(realPart / ReflectionRatio.magnitude);
    }

    //qDebug() << realPart << " " << imagPart;
    //qDebug() << ReflectionRatio.magnitude << " " << ReflectionRatio.phase_rad;

    return ReflectionRatio;
}

bool widgetSmith::eventFilter(QObject *, QEvent *event)
{
    bool state = false;

    if(event->type() == QEvent::Resize)
    {
        repaint();
        state = true;
    }
    else
    {
        state = false;

    }
    return state;
}

void widgetSmith::paintEvent(QPaintEvent*)
{
    QPainter painterMain(this);

    clearButton->setGeometry(width() / 2, height() - 50, 50, 20);
    nmbDisplayedSamples->setGeometry(width() / 2, height() - 25, 50, 20);

    painterMain.drawPixmap(0, 0, QPixmap(QString(":/smith.png")).scaled(size()));


    painterMain.setPen(QPen(Qt::blue));
    painterMain.setBrush(QBrush(Qt::blue));

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        qreal xSave = reflFinishAlg[iLoop].real;
        qreal ySave = reflFinishAlg[iLoop].imag;

        QPointF adjustSize((width() / 4) + (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
        painterMain.drawEllipse(adjustSize,3,3);
    }


    for(int jLoop = 0; jLoop < 3; jLoop++)
    {
        if(axis[jLoop].count() >= int(currentNmbPoint))
        {
            axis[jLoop].removeFirst();
        }

        for(int iLoop = 0; iLoop < axis[jLoop].count(); iLoop++)
        {
            painterMain.setPen(QPen(Qt::blue));
            qreal xSave = axis[jLoop].at(iLoop).x() * cos(axis[jLoop].at(iLoop).y());
            qreal ySave = axis[jLoop].at(iLoop).x() * sin(axis[jLoop].at(iLoop).y());

            //qDebug() << "values: " << xSave << "and" << ySave;

            if(jLoop == 0)
            {
                painterMain.setBrush(QBrush(Qt::blue));
                QPointF adjustSize((width() / 4) + (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
                painterMain.drawEllipse(adjustSize,3,3);
            }
            else if(jLoop == 1)
            {
                painterMain.setBrush(QBrush(Qt::red));
                QPointF adjustSize((width() / 4) + (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
                painterMain.drawEllipse(adjustSize,3,3);
            }
            else if(jLoop == 2)
            {
                painterMain.setBrush(QBrush(Qt::green));
                QPointF adjustSize((width() * 3 / 4) + (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
                painterMain.drawEllipse(adjustSize,3,3);
            }
        }

    }

    painterMain.setPen(QPen(Qt::blue));
    painterMain.drawText(QPoint(width() / 2, 20), QString("average x 50%1").arg(QChar(0x03A9)));
    painterMain.setPen(QPen(Qt::red));
    painterMain.drawText(QPoint(width() / 2, 40), QString("current x 50%1").arg(QChar(0x03A9)));
    painterMain.setPen(QPen(Qt::green));
    painterMain.drawText(QPoint(width() / 2, 60), "current x average");
}
