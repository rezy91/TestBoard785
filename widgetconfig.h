#ifndef widgetConfig_H
#define widgetConfig_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class widgetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit widgetConfig(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:

public slots:

private:
    QSize currSize;

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetConfig_H
