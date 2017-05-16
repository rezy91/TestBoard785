#include "widgetone.h"

widgetOne::widgetOne(QWidget *parent) : QWidget(parent)
{

}

void widgetOne::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawText(QPoint(10, 10), QString("One"));

}
