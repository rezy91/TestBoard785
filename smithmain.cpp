#include "smithmain.h"

#include <QtGui>
#include <QtCore>

SmithMain::SmithMain(QWidget *parent) : QMainWindow(parent)
{
    installEventFilter(this);
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

    painterMain.drawPixmap(0, 0, QPixmap("smith.png").scaled(size()));

    for(int jLoop = 0; jLoop < 3; jLoop++)
    {
        if(axis[jLoop].count() >= 200)
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
