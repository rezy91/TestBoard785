#ifndef widgetGraph_H
#define widgetGraph_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class widgetGraph : public QWidget
{
    Q_OBJECT
public:
    explicit widgetGraph(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}
    void SetQStart(QSize value) {currStart = value;}

signals:

public slots:

private:
    QSize currSize;
    QSize currStart;

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetGraph_H
