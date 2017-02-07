#include "smithmain.h"

#include <QtGui>
#include <QtCore>

SmithMain::SmithMain(QWidget *parent) : QMainWindow(parent)
{
    installEventFilter(this);

    clearButton->setText("Clear");
    nmbDisplayedSamples->setMinimum(1);
    nmbDisplayedSamples->setMaximum(9999);
    nmbDisplayedSamples->setValue(100);
    currentNmbPoint = nmbDisplayedSamples->value();


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
        qDebug() << "value changed:" << newValue;


    });


}

quint32 SmithMain::GetNmbPoints()
{
    return nmbDisplayedSamples->value();
}

void SmithMain::SetNmbPoints(quint32 value)
{
    nmbDisplayedSamples->setValue(value);
    currentNmbPoint = value;
}

void SmithMain::ReceivedNewData(qreal magnitudeCurrAvg, qreal phaseCurrAvg, qreal magnitudeCurr50, qreal phaseCurr50, qreal magnitudeAvg50, qreal phaseAvg50)
{
    axis[0].append(QPointF(magnitudeAvg50, phaseAvg50));
    axis[1].append(QPointF(magnitudeCurr50, phaseCurr50));
    axis[2].append(QPointF(magnitudeCurrAvg, phaseCurrAvg));

    repaint();
}

bool SmithMain::eventFilter(QObject *, QEvent *event)
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

void SmithMain::paintEvent(QPaintEvent*)
{
    QPainter painterMain(this);

    clearButton->setGeometry(width() / 2, height() - 50, 50, 20);
    nmbDisplayedSamples->setGeometry(width() / 2, height() - 25, 50, 20);


    painterMain.drawPixmap(0, 0, QPixmap("smith.png").scaled(size()));

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
