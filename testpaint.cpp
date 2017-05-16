#include "testpaint.h"

TestPaint::TestPaint(QWidget *parent) : QWidget(parent)
{
}

void TestPaint::SetText(QString inputText)
{
    pButt->setText(inputText);
    pButt->show();
}

void TestPaint::SetSize(int valueToSize)
{
    size = valueToSize;
}

void TestPaint::paintEvent(QPaintEvent* e) {

    Q_UNUSED(e);


    QPainter pp(this);

    pp.begin(this);
    pp.setPen(QPen(Qt::red, 3));
    for (int ivar = 0; ivar < size; ++ivar) {
        for (int jvar = 0; jvar < size; ++jvar) {
            pp.drawPoint(QPointF(50+ivar, 50+jvar));
        }
    }


    pp.end();
}
