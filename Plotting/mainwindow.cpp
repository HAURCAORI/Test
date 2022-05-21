#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QLabel* MainWindow::getView(QString id) {
    QList<QLabel*> temp = this->findChildren<QLabel*>(id);
    if(temp.empty()) { return nullptr; }
    return temp[0];
}

void MainWindow::setImage(QLabel* widget, View* view) {
    widget->resize(view->width, view->height);
    int bytes_per_line = view->width * CHANNEL;
    QImage image( view->data, view->width, view->height, bytes_per_line, FORMATQT );
    widget->setPixmap(QPixmap::fromImage(image));
}
