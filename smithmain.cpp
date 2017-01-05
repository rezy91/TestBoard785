#include "smithmain.h"

#include <QtGui>
#include <QtCore>

SmithMain::SmithMain(QWidget *parent) : QMainWindow(parent)
{
    showMaximized();
    installEventFilter(this);

}

void SmithMain::ReceivedNewData(int magnitude, int phase)
{
    if(!bEnableDraw)
    {
        bEnableDraw = true;
    }
    mRatio_magnitude = qreal(magnitude) / 1000;
    mRatio_phase = qreal(phase) / 1000;

    update();
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
/*
        int Max = 353;
        int Min = -353;
        int randNumberX = ((rand()%(Max-Min+1))+Min);
        int randNumberY = ((rand()%(Max-Min+1))+Min);
        QPointF center(450 + randNumberX, 450 + randNumberY);
        centers.append(center);*/


        /*if(abs(mRatio_magnitude) > 0.05)
        {*/
            QPointF centerLeft(mRatio_magnitude,mRatio_phase);
            centers.append(centerLeft);

/*
            QPointF centerRight(mRatio_magnitude,mRatio_phase);
            centers.append(centerRight);*/
        //}

        if(centers.count() >= 1000)
        {
            centers.clear();
        }
        else
        {
            for(int iLoop = 0; iLoop < centers.count(); iLoop++)
            {
                painterMain.setPen(QPen(Qt::blue));
                painterMain.setBrush(QBrush(Qt::red));
                qreal xSave = centers.at(iLoop).x();
                qreal ySave = centers.at(iLoop).y();

                //qDebug() << "values: " << xSave << " and " << ySave;

                QPointF adjustSize((width() / 2) + (xSave * width() / 2), (height() / 2) + (ySave * height() / 2));
                painterMain.drawEllipse(adjustSize,3,3);
            }
        }
    }
}
