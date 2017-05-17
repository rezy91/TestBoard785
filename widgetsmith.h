#ifndef widgetSmith_H
#define widgetSmith_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class widgetSmith : public QWidget
{
    Q_OBJECT
public:
    explicit widgetSmith(QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetSmith_H
