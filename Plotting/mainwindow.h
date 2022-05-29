#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <iostream>
#include "drawing.h"
#include "qplot.h"
#include "rtthread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    qplot* getViewWidget(QString id);
    void setImage(QLabel* widget, const View* view);
    void setEvent(QLabel* target);
    RTThread* rtthread;

private slots:
    void Mouse_Down();
    void Mouse_Up();
    void Mouse_left();
    void Update_Image(QString);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
