#include "qplot.h"

qplot::qplot(QWidget *parent) : QLabel(parent)
{

}

void qplot::mouseMoveEvent(QMouseEvent *event)
{
    if(is_press) {
        this->x = event->x();
        this->y = event->y();

        if(inArea(this->x, this->y, plot.plotSize(),plot.plotLocation())) {

        }
        plot.movePlot(x-x0,y-y0);
        x0 = this->x;
        y0 = this->y;
        updateImage();
    }
    emit Mouse_Pos();
}

void qplot::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        x0 = event->x();
        y0 = event->y();
        is_press = true;

        emit Mouse_Down();
        updateImage();
    }
}

void qplot::mouseReleaseEvent(QMouseEvent *event) {
    is_press = false;
    emit Mouse_Up();
}

void qplot::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        plot.moveOrigin();
        updateImage();
    }

}

void qplot::leaveEvent(QEvent *event)
{
    emit Mouse_Left();
}

void qplot::updateImage()
{
    if(!view) { return; }
    int bytes_per_line = view->width * CHANNEL;
    QImage image( view->data, view->width, view->height, bytes_per_line, FORMATQT );
    this->setPixmap(QPixmap::fromImage(image));
}

void qplot::init()
{
    plot = rtplot::rtplot(this->width(),this->height());
    view = plot.getView();
    updateImage();
}
