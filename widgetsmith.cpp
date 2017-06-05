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

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        complex_N o_ReflRatio(complex_N::GONIO, f_MaxReflRatioReference, ((2 * 3.14159265358) / NMB_PHASES) * iLoop);
        complex_N o_Fraction = (complex_N(complex_N::ALGEB, 1, 0) + o_ReflRatio) / (complex_N(complex_N::ALGEB, 1, 0) - o_ReflRatio);
        complex_N o_Result = m_ReferenceImpedance * o_Fraction;
        reflFinishAlg[iLoop] = CalculateReflectionRatio(o_Result, complex_N(complex_N::GONIO, 50, 0));
    }

    painterMain.setPen(QPen(Qt::darkGray));
    painterMain.setBrush(QBrush(Qt::darkGray));

    for(int iLoop = 0; iLoop < NMB_PHASES; iLoop++)
    {
        qreal xSave = reflFinishAlg[iLoop].GetReal();
        qreal ySave = reflFinishAlg[iLoop].GetImag();

        QPointF adjustSize((width() / 4) + (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
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
