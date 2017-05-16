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

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // WIDGETTWO_H
