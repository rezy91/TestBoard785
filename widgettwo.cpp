#include "widgettwo.h"

widgetTwo::widgetTwo(QWidget *parent) : QWidget(parent)
{

}

void widgetTwo::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawText(QPoint(10, 10), QString("Two"));

}
