#include "widgetone.h"

widgetOne::widgetOne(QWidget *parent) : QWidget(parent)
{

}

void widgetOne::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawLine(QPointF(0,0),QPointF(currSize.width(),currSize.height()));

}
