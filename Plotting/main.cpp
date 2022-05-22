#include "mainwindow.h"
#include <QApplication>
#include "rtplot.h"
#include "qplot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


/*
    //std::vector<INT> vec = {1,2,3,4,5};
    std::vector<FLOAT> vec = {1,2,3,4,5};
    std::vector<STRING> vec2 = {"a","ab","abc","abcd","abcde"};
    std::vector<FloatFloat> vec3 = {{1,2},{3,4},{5,6}};
    std::vector<StringFloat> vec4 = {{"A",2},{"b",4},{"C",6}};
    rtplot::DataSet ds;
    ds.addData("a", &vec);
    ds.addData("b", &vec2);
    ds.addData("c", &vec3);
    ds.addData("d", &vec4);
    ds.printAll();
*/
    /*
    View view1(800, 600, FORMAT);
    Simd::Fill(view1,255);
    Simd::Font font(10);
    font.Draw(view1,"abc",Point(10,10),Color(0, 0, 255));
    Simd::DrawLine(view1,Point(0,0),Point(10,10),Color(255,0,0));


    View view2(100, 100, FORMAT);
    //Simd::FillValueBgra(view2,255,0,0,255);
    //SimdFill(view2.data,4*500,500,200,4,100);


    for(int i = 0 ; i < 500*200; i+=1)
    {
        view2.data[i*4] = 255;
        view2.data[i*4+1] = 0;
        view2.data[i*4+2] = 0;
        view2.data[i*4+3] = 255;
    }


    Overlay(view1,100,100,view2);
*/
    rtplot::rtplot plot = rtplot::rtplot(500,400);
    plot.init();
    const View* view1 = plot.getView();

    w.setImage(w.getView("view1"), view1);

    return a.exec();
}
