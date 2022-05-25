#include "qplot.h"

qplot::qplot(QWidget *parent) : QLabel(parent)
{

}

void qplot::mouseMoveEvent(QMouseEvent *event)
{
    if(is_press) {
        this->x = event->x();
        this->y = event->y();

        if(area == Area::plot) {
            plot.movePlot(x-x0,y-y0);
        } else if(area == Area::x_axis) {
            if(is_mouse_left) plot.movePlot(x-x0,0);
            if(is_mouse_right) plot.scalePlot(plot.xAxis(),point_value, y-y0);
        } else if(area == Area::y_axis) {
            if(is_mouse_left) plot.movePlot(0,y-y0);
            if(is_mouse_right) plot.scalePlot(plot.yAxis(),point_value, x0-x);
        }
        x0 = this->x;
        y0 = this->y;
        updateImage();
    }
    emit Mouse_Pos();
}

void qplot::mousePressEvent(QMouseEvent *event)
{
    x0 = event->x();
    y0 = event->y();
    if(inArea(x0, y0, plot.plotSize(),plot.plotLocation())) {
        area = Area::plot;
    } else if(inArea(x0, y0, plot.xAxisSize(),plot.xAxisLocation())) {
        rtplot::Gradation *g =plot.xAxis();
        point_value = g->min_value + (g->max_value-g->min_value)/(g->size.width-g->margin*2)*(x0 - g->location.x - g->margin);
        area = Area::x_axis;
    } else if(inArea(x0, y0, plot.yAxisSize(),plot.yAxisLocation())) {
        rtplot::Gradation *g =plot.yAxis();
        point_value = g->min_value + (g->max_value-g->min_value)/(g->size.height-g->margin*2)*(g->size.height - y0 + g->location.y - g->margin);
        area = Area::y_axis;
    } else {
        area = Area::none;
    }

    if(event->buttons() == Qt::LeftButton) {
        is_press = true;
        is_mouse_left = true;


        emit Mouse_Down();
        updateImage();
    } else if(event->buttons() == Qt::RightButton) {
        is_press = true;
        is_mouse_right = true;

        updateImage();
    }
}

void qplot::mouseReleaseEvent(QMouseEvent *event) {
    is_press = false;
    is_mouse_left = false;
    is_mouse_right = false;
    area = Area::none;
    emit Mouse_Up();
}

void qplot::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        if(inArea(x0, y0, plot.plotSize(),plot.plotLocation())) {
            plot.moveOrigin(plot.xAxis());
            plot.moveOrigin(plot.yAxis());
        } else if(inArea(x0, y0, plot.xAxisSize(),plot.xAxisLocation())) {
            plot.moveOrigin(plot.xAxis());
        } else if(inArea(x0, y0, plot.yAxisSize(),plot.yAxisLocation())) {
            plot.moveOrigin(plot.yAxis());
        }
        updateImage();
    }
    if(event->buttons() == Qt::RightButton) {
        if(inArea(x0, y0, plot.xAxisSize(),plot.xAxisLocation())) {
            plot.scaleOrigin(plot.xAxis(),point_value);
        } else if(inArea(x0, y0, plot.yAxisSize(),plot.yAxisLocation())) {
            plot.scaleOrigin(plot.yAxis(),point_value);
        }
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
