#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setEvent(ui->view1);
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

void MainWindow::setImage(QLabel* widget, const View* view) {
    widget->resize(view->width, view->height);
    int bytes_per_line = view->width * CHANNEL;
    QImage image( view->data, view->width, view->height, bytes_per_line, FORMATQT );
    widget->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::setEvent(QLabel* target) {
    if(!target) return;
    connect(target, SIGNAL(Mouse_Pos()),this,SLOT(Mouse_current_pos()));
    connect(target, SIGNAL(Mouse_Down()),this,SLOT(Mouse_Down()));
    connect(target, SIGNAL(Mouse_Up()),this,SLOT(Mouse_Up()));
    connect(target, SIGNAL(Mouse_Left()),this,SLOT(Mouse_left()));
}

void MainWindow::Mouse_current_pos()
{
    std::cout<<ui->view1->x << "," << ui->view1->y << std::endl;
}

void MainWindow::Mouse_Down()
{
    std::cout << "down" << std::endl;
}

void MainWindow::Mouse_Up()
{
    std::cout << "up" << std::endl;
}

void MainWindow::Mouse_left()
{
    std::cout << "leave" << std::endl;
}
