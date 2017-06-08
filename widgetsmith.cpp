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

#include <QtGui>
#include <QtCore>

widgetSmith::widgetSmith(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);

    clearButton->setText("Clear");
    nmbDisplayedSamples->setMinimum(1);
    nmbDisplayedSamples->setMaximum(9999);


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

void widgetSmith::ReceivedNewData(qreal magnitudeCurrAvg, qreal phaseCurrAvg, qreal magnitudeCurr50, qreal phaseCurr50)
{
    complex_N m_ImpAverage(complex_N::GONIO, magnitudeCurrAvg, acos(phaseCurrAvg));
    complex_N m_ImpCurrent(complex_N::GONIO, magnitudeCurr50, acos(phaseCurr50));
    complex_N m_Imp50Ohm(complex_N::ALGEB, 50, 0);

    axis[0].append(CalculateReflectionRatio(m_ImpAverage, m_Imp50Ohm));
    axis[1].append(CalculateReflectionRatio(m_ImpCurrent, m_Imp50Ohm));
    axis[2].append(CalculateReflectionRatio(m_ImpCurrent, m_ImpAverage));

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        complex_N o_ReflRatioRef(complex_N::GONIO, f_MaxReflRatioReference, ((2 * 3.14159265358) / NMB_PHASES) * iLoop);
        complex_N o_ReflRatioAvg(complex_N::GONIO, f_MaxReflRatioCurrent, ((2 * 3.14159265358) / NMB_PHASES) * iLoop);

        complex_N o_FractionRef = (complex_N(complex_N::ALGEB, 1, 0) + o_ReflRatioRef) / (complex_N(complex_N::ALGEB, 1, 0) - o_ReflRatioRef);
        complex_N o_FractionAvg = (complex_N(complex_N::ALGEB, 1, 0) + o_ReflRatioAvg) / (complex_N(complex_N::ALGEB, 1, 0) - o_ReflRatioAvg);

        complex_N o_ResultRef = m_ReferenceImpedance * o_FractionRef;
        complex_N o_ResultAvr = m_ImpAverage * o_FractionAvg;

        limitsRef[iLoop] = CalculateReflectionRatio(o_ResultRef, m_Imp50Ohm);
        limitsAvg[iLoop] = CalculateReflectionRatio(o_ResultAvr, m_Imp50Ohm);
    }

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

void widgetSmith::ReadReferenceImpedance(float magnitude, float phase, float maxReflRatioRef, float maxReflRatioCur)
{
    m_ReferenceImpedance = complex_N(complex_N::GONIO, magnitude, phase);
    f_MaxReflRatioReference = maxReflRatioRef;
    f_MaxReflRatioCurrent = maxReflRatioCur;
}

complex_N widgetSmith::CalculateReflectionRatio(complex_N current, complex_N average)
{
    return ((current - average) / (current + average));
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

    painterMain.setPen(QPen(Qt::darkGray));
    painterMain.setBrush(QBrush(Qt::darkGray));

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        QPointF adjustSize((width() / 4) + (limitsRef[iLoop].GetReal() * width() * 7 / (4 * 9)), (height() / 2) - (limitsRef[iLoop].GetImag() * height() * 7 / (2 * 9)));
        painterMain.drawEllipse(adjustSize, 2, 2);
    }

    painterMain.setPen(QPen(Qt::black));
    painterMain.setBrush(QBrush(Qt::black));

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        QPointF adjustSize((width() / 4) + (limitsAvg[iLoop].GetReal() * width() * 7 / (4 * 9)), (height() / 2) - (limitsAvg[iLoop].GetImag() * height() * 7 / (2 * 9)));
        painterMain.drawEllipse(adjustSize, 2, 2);
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
            qreal xSave = axis[jLoop][iLoop].GetReal();
            qreal ySave = axis[jLoop][iLoop].GetImag();


            //qDebug() << "values: " << xSave << "and" << ySave;
            QPointF adjustSize;

            if(jLoop == 0)
            {
                painterMain.setBrush(QBrush(Qt::blue));
                adjustSize.setX((width() / 4) + (xSave * width() * 7 / (4 * 9)));
                adjustSize.setY((height() / 2) - (ySave * height() * 7 / (2 * 9)));
            }
            else if(jLoop == 1)
            {
                painterMain.setBrush(QBrush(Qt::red));
                adjustSize.setX((width() / 4) + (xSave * width() * 7 / (4 * 9)));
                adjustSize.setY((height() / 2) - (ySave * height() * 7 / (2 * 9)));
            }
            else if(jLoop == 2)
            {
                painterMain.setBrush(QBrush(Qt::green));
                adjustSize.setX((width() * 3 / 4) + (xSave * width() * 7 / (4 * 9)));
                adjustSize.setY((height() / 2) - (ySave * height() * 7 / (2 * 9)));
            }

            if(!(std::isnan(adjustSize.rx()) || std::isnan(adjustSize.rx())))
            {
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
