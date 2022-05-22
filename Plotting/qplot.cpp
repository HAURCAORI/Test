#include "qplot.h"

qplot::qplot(QWidget *parent) : QLabel(parent)
{

}

void qplot::mouseMoveEvent(QMouseEvent *event)
{
    this->x = event->x();
    this->y = event->y();
    emit Mouse_Pos();
}

void qplot::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        emit Mouse_Down();
    }
}

void qplot::mouseReleaseEvent(QMouseEvent *event) {
    emit Mouse_Up();
}

void qplot::leaveEvent(QEvent *event)
{
    emit Mouse_Left();
}
