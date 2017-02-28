#ifndef SMITHMAIN_H
#define SMITHMAIN_H

#include <QMainWindow>
#include <QList>
#include <QObject>
#include <QEvent>
#include <QPushButton>
#include <QSpinBox>

class SmithMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit SmithMain(QWidget *parent = 0);

    quint32 GetNmbPoints(void);
    void SetNmbPoints(quint32 value);

public slots:
    void ReceivedNewData(qreal magnitudeCurrAvg, qreal phaseCurrAvg, qreal magnitudeCurr50, qreal phaseCurr50, qreal magnitudeAvg50, qreal phaseAvg50);

private:
    QList<QPointF> axis[3];

    QPushButton* clearButton = new QPushButton(this);
    QSpinBox* nmbDisplayedSamples = new QSpinBox(this);

    quint32 currentNmbPoint;

protected:
    void paintEvent(QPaintEvent*);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // SMITHMAIN_H
