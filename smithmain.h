#ifndef SMITHMAIN_H
#define SMITHMAIN_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>

class SmithMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit SmithMain(QWidget *parent = 0);
public slots:
    void ReceivedNewData(qreal magnitudeCurrAvg, qreal phaseCurrAvg, qreal magnitudeCurr50, qreal phaseCurr50, qreal magnitudeAvg50, qreal phaseAvg50);

private:
    QList<QPointF> axis[3];

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // SMITHMAIN_H
