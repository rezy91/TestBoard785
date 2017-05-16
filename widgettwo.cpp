#include "widgettwo.h"

widgetTwo::widgetTwo(QWidget *parent) : QWidget(parent)
{

}

void widgetTwo::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawLine(QPointF(0,currSize.height()),QPointF(currSize.width(),0));

}
