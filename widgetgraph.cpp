#include "widgetgraph.h"
#include <QCoreApplication>

widgetGraph::widgetGraph(QWidget *parent) : QWidget(parent)
{

}

void widgetGraph::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    QString pathToFile = QString("%1/").arg(QCoreApplication::applicationDirPath());
    pathToFile += "smith.png";

    painter.drawPixmap(0, 0, QPixmap(pathToFile).scaled(size()));

    //qDebug() << "Min pathToFile : " << pathToFile;

}
