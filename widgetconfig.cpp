#include "widgetconfig.h"

widgetConfig::widgetConfig(QWidget *parent) : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::green);
    setAutoFillBackground(true);
    setPalette(pal);
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.drawLine(QPointF(0,0),QPointF(currSize.width(),currSize.height()));

}
