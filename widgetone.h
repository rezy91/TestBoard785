#ifndef WIDGETONE_H
#define WIDGETONE_H

#include <QObject>
#include <QWidget>
#include <QPainter>

class widgetOne : public QWidget
{
    Q_OBJECT
public:
    explicit widgetOne(QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // WIDGETONE_H
