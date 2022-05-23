#ifndef QPLOT_H
#define QPLOT_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include "rtplot.h"

class qplot : public QLabel
{
    Q_OBJECT
private:
    int x0, y0;
    bool is_press = false;
public:
    explicit qplot(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void updateImage();
    void init();

    rtplot::rtplot plot;
    const View* view;
    int x,y;
signals:
    void Mouse_Down();
    void Mouse_Up();
    void Mouse_Pos();
    void Mouse_Left();

public slots:
};

#endif // QPLOT_H
