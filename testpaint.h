#ifndef TESTPAINT_H
#define TESTPAINT_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QPainter>

class TestPaint : public QWidget
{
    Q_OBJECT
public:
    explicit TestPaint(QWidget *parent = 0);
    void SetText(QString inputText);
    void SetSize(int valueToSize);
private:
    QPushButton* pButt = new QPushButton(this);
    int size;
protected:
    virtual void paintEvent(QPaintEvent*e);

};

#endif // TESTPAINT_H
