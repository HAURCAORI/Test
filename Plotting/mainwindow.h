#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <iostream>
#include "drawing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QLabel* getView(QString id);
    void setImage(QLabel* widget, View* view);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
