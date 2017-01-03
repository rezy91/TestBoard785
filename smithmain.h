#ifndef SMITHMAIN_H
#define SMITHMAIN_H

#include <QMainWindow>
#include <QLabel>

class SmithMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit SmithMain(QWidget *parent = 0);

private:
    QLabel *wdg = new QLabel;
protected:
    void paintEvent(QPaintEvent* e);
};

#endif // SMITHMAIN_H
