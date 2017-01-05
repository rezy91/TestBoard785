#ifndef GRAPMAIN_H
#define GRAPMAIN_H

#include <QMainWindow>
#include <QList>
#include <QCheckBox>
#include <QHBoxLayout>


class Grapmain : public QMainWindow
{
    Q_OBJECT
public:
    enum{nmbCurvesInGraph = 4};

    explicit Grapmain(QWidget *parent = 0);

private:
    bool bEnableDraw = false;

public slots:
    void Refresh(int someValue);

protected:
    void paintEvent(QPaintEvent*);
};

#endif // GRAPMAIN_H
