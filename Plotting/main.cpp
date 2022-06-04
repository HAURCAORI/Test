#include "mainwindow.h"
#include <QApplication>
#include <rtplot.h>
#include <plotstruct.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <thread>
#include <future>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.getViewWidget("view1")->init();
    w.EnableThread(w.getViewWidget("view1"));
    //DataIO::IPCSharedMemory ipc(DataIO::IPC_MODE::RECEIVER, 98765);
    //QtConcurrent::run(task,ipc, &w);


    rtplot::DataSet ds;
    //std::vector<INT> vec = {1,2,3,4,5};
    std::vector<FLOAT> vec = {1,2,3,4,5.1,4.2,1.3};
    std::vector<STRING> vec2 = {"a","ab","abc","abcd","abcde"};
    std::vector<FloatFloat> vec3 = {{1,2},{3,4},{5,6}};
    std::vector<StringFloat> vec4 = {{"A",2},{"b",4},{"C",6}};

    //ds.addData("single", &vec);
    //ds.addData("single2", &vec);
    //ds.addData("b", &vec2);
    //ds.addData("c", &vec3);
    //ds.addData("d", &vec4);
    //ds.printAll();
    std::vector<rtplot::DataStruct> aa;
    aa.push_back(rtplot::DataStruct("A",rtplot::DataType::SINGLE_FLOAT,&vec,vec.size(),sizeof(FLOAT)));
    w.getViewWidget("view1")->setDataSet(ds,rtplot::DataType::SINGLE_FLOAT);
    //w.getViewWidget("view1")->updateDataSet(&aa);
    w.startThread();

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


/*
    rtplot::rtplot plot = rtplot::rtplot(500,400);
    rtplot::rtplot plot1(std::move(plot));
    const View* view1 = plot1.getView();

    w.setImage(w.getView("view1"), view1);
*/
    return a.exec();
}
