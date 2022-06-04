#ifndef QPLOT_H
#define QPLOT_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include "rtplot.h"

enum class Area {
    none,
    title,
    plot,
    x_axis,
    y_axis
};

class qplot : public QLabel
{
    Q_OBJECT
private:
    int x0, y0;
    double point_value;
    Area area;
    bool is_press = false;
    bool is_mouse_left = false;
    bool is_mouse_right = false;

    inline bool inArea(int x, int y, Size size, Location location) {
        if(x >= (int) location.x && x < (int) (location.x + size.width) && y >= (int) location.y && y < (int) (location.y + size.height)) {
            return true;
        }
        return false;
    }
    rtplot::rtplot plot;

public:
    explicit qplot(QWidget *parent = nullptr);
    ~qplot();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void updateImage();
    void init();
    void setDataSet(rtplot::DataSet dataset, rtplot::DataType type);
    rtplot::DataSet* getDataSet() { return plot.getDataSet(); }
    void updatePlot();
    void updateDataSet(std::vector<rtplot::DataStruct>* rds);

    const View* view = nullptr;
    int x,y;
    bool update = true;
signals:
    void Mouse_Down();
    void Mouse_Up();
    void Mouse_Pos();
    void Mouse_Left();

public slots:
};

#endif // QPLOT_H
