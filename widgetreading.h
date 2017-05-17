#ifndef widgetReading_H
#define widgetReading_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class widgetReading : public QWidget
{
    Q_OBJECT
public:
    explicit widgetReading(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:

public slots:

private:
    QSize currSize;

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetReading_H
