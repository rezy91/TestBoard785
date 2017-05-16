#include "widgetthree.h"

widgetThree::widgetThree(QWidget *parent) : QWidget(parent)
{

}

void widgetThree::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawText(QPoint(10, 10), QString("Three"));


    //painter.drawPixmap(0, 0, QPixmap("smith.png").scaled(size()));
}
