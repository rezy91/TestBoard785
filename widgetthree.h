#ifndef WIDGETTHREE_H
#define WIDGETTHREE_H

#include <QObject>
#include <QWidget>
#include <QPainter>

class widgetThree : public QWidget
{
    Q_OBJECT
public:
    explicit widgetThree(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:

public slots:

private:
    QSize currSize;

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // WIDGETTHREE_H
