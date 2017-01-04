#ifndef SMITHMAIN_H
#define SMITHMAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QImage>

class SmithMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit SmithMain(QWidget *parent = 0);

private:

    QImage *image = new QImage(900,900,QImage::Format_RGB888);

protected:
    void paintEvent(QPaintEvent* e);
};

#endif // SMITHMAIN_H
