#include "mainwindow.h"
#include <QApplication>
#include "rtplot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

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
/*
    View view1(400, 300, FORMAT);
    Simd::Fill(view1,255);
    Simd::Font font(10);
    font.Draw(view1,"abc",Point(10,10),Color(0, 0, 255));
    Simd::DrawLine(view1,Point(0,0),Point(10,10),Color(255,0,0));

    View view2(400, 300, FORMAT);
    Simd::Fill(view2,100);
    //Overlay(view1,10,10,view2);


    w.setImage(w.getView("view1"),&view1);
*/
    return a.exec();
}
