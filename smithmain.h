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
    void ReceivedNewData(int magnitude, int phase);

private:
    qreal mRatio_magnitude;
    qreal mRatio_phase;

    bool bEnableDraw = false;

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // SMITHMAIN_H
