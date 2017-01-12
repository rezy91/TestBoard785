#include "smithmain.h"

#include <QtGui>
#include <QtCore>

SmithMain::SmithMain(QWidget *parent) : QMainWindow(parent)
{
    installEventFilter(this);
}

void SmithMain::ReceivedNewData(int magnitudeAvg, int phaseAvg, int magnitude50, int phase50)
{
    if(!bEnableDraw)
    {
        bEnableDraw = true;
    }
    mRatio_magnitudeAvg = qreal(magnitudeAvg) / 1000;
    mRatio_phaseAvg = qreal(phaseAvg) / 1000;
    mRatio_magnitude50 = qreal(magnitude50) / 1000;
    mRatio_phase50 = qreal(phase50) / 1000;

    update();
}

void SmithMain::ReceivedStateButton(bool state)
{
    if(state)
    {
        setFixedSize(width(), height());
    }
    else
    {
        setMinimumSize(0,0);
        setMaximumSize(10000,10000);
    }
}

bool SmithMain::eventFilter(QObject *, QEvent *event)
{
    static int counter = 0;
    bool state = false;

    if(event->type() == QEvent::Resize)
    {
        qDebug() << "resize " << counter++;
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
    static QList<QPointF> centers;
    QPainter painterMain(this);

    painterMain.drawPixmap(0, 0, QPixmap("smith.png").scaled(size()));

    if(bEnableDraw)
    {
        if(centers.count() >= 200)
        {
            centers.removeFirst();
            centers.removeFirst();
        }

        QPointF centerLeft(mRatio_magnitudeAvg,mRatio_phaseAvg);
        centers.append(centerLeft);

        QPointF centerRight(mRatio_magnitude50,mRatio_phase50);
        centers.append(centerRight);

        for(int iLoop = 0; iLoop < centers.count(); iLoop++)
        {
            painterMain.setPen(QPen(Qt::blue));
            painterMain.setBrush(QBrush(Qt::red));
            qreal xSave = centers.at(iLoop).x();
            qreal ySave = centers.at(iLoop).y();

            //qDebug() << "values: " << xSave << " and " << ySave;

            if(iLoop % 2)
            {
                QPointF adjustSize((width() / 4) - (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
                painterMain.drawEllipse(adjustSize,3,3);
            }
            else
            {
                QPointF adjustSize((width() * 3 / 4) - (xSave * width() * 7 / (4 * 9)), (height() / 2) - (ySave * height() * 7 / (2 * 9)));
                painterMain.drawEllipse(adjustSize,3,3);
            }
        }
    }
}
