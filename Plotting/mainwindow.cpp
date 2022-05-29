#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setEvent(ui->view1);

    rtthread = new RTThread(this);
    QObject::connect(rtthread, SIGNAL(Update_Image(QString)),this,SLOT(Update_Image(QString)),Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

qplot* MainWindow::getViewWidget(QString id) {
    QList<qplot*> temp = this->findChildren<qplot*>(id);
    if(temp.empty()) { return nullptr; }
    return temp[0];
}

void MainWindow::setImage(QLabel* widget, const View* view) {
    widget->resize(view->width, view->height);
    int bytes_per_line = view->width * CHANNEL;
    QImage image( view->data, view->width, view->height, bytes_per_line, FORMATQT );
    widget->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::setEvent(QLabel* target) {
    if(!target) return;
    connect(target, SIGNAL(Mouse_Down()),this,SLOT(Mouse_Down()));
    connect(target, SIGNAL(Mouse_Up()),this,SLOT(Mouse_Up()));
    connect(target, SIGNAL(Mouse_Left()),this,SLOT(Mouse_left()));

}

void MainWindow::Mouse_Down()
{

}

void MainWindow::Mouse_Up()
{

}

void MainWindow::Mouse_left()
{

}

void MainWindow::Update_Image(QString id){
    getViewWidget(id)->updatePlot();
}
