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
    void ReceivedNewData(int magnitudeAvg, int phaseAvg, int magnitude50, int phase50);
    void ReceivedStateButton(bool state);

private:
    qreal mRatio_magnitudeAvg;
    qreal mRatio_phaseAvg;
    qreal mRatio_magnitude50;
    qreal mRatio_phase50;

    bool bEnableDraw = false;

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // SMITHMAIN_H
