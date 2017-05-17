#include "widgetsmith.h"

widgetSmith::widgetSmith(QWidget *parent) : QWidget(parent)
{    
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red);
    setAutoFillBackground(true);
    setPalette(pal);
}

void widgetSmith::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);


}
