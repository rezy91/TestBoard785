#ifndef WIDGETTWO_H
#define WIDGETTWO_H

#include <QObject>
#include <QWidget>
#include <QPainter>

class widgetTwo : public QWidget
{
    Q_OBJECT
public:
    explicit widgetTwo(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:

public slots:

private:
    QSize currSize;

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // WIDGETTWO_H
