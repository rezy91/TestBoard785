#include "widgetreading.h"

widgetReading::widgetReading(QWidget *parent) : QWidget(parent)
{

}

void widgetReading::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawLine(QPointF(0,currSize.height()),QPointF(currSize.width(),0));

}
