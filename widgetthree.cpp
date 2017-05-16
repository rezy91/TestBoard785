#include "widgetthree.h"

widgetThree::widgetThree(QWidget *parent) : QWidget(parent)
{

}

void widgetThree::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawLine(QPointF(currSize.width()/2,0),QPointF(currSize.width()/2,currSize.height()));

}
