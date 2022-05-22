#include "drawing.h"
#include <iostream>
#include <type_traits>

#ifndef RTPLOT_H
#define RTPLOT_H

typedef int INT;
typedef float FLOAT;
typedef std::string STRING;

template<typename T, typename U>
struct Pair {
    T first;
    U second;
};

typedef Pair<FLOAT,FLOAT> FloatFloat;
typedef Pair<STRING,FLOAT> StringFloat;
typedef Pair<STRING,INT> StringInt;

template<typename T>
inline std::string getType() {
    if(typeid(T) == typeid(INT)) { return "int"; }
    else if(typeid(T) == typeid(FLOAT)) { return "float"; }
    else if(typeid(T) == typeid(STRING)) { return "string"; }
    else return typeid(T).name();
}

template<typename T>
inline void printVector(const std::vector<T> vec) {
    std::cout << getType<T>() << ":\t";
    for(auto it = vec.begin(); it != vec.end(); ++it ) {
        std::cout << *it << '\t';
    }
    std::cout << std::endl;
}
template<typename T, typename U>
inline void printVector(const std::vector<Pair<T,U>> vec) {
    std::cout << getType<T>() << ":\t";
    for(auto it = vec.begin(); it != vec.end(); ++it ) {
        std::cout << (*it).first << '\t';
    }
    std::cout << std::endl << getType<U>() << ":\t";
    for(auto it = vec.begin(); it != vec.end(); ++it ) {
        std::cout << (*it).second << '\t';
    }
    std::cout << std::endl;
}

namespace rtplot {


enum class DataType {
    SINGLE_INT,
    SINGLE_FLOAT,
    SINGLE_STRING,
    PAIR_STRING_INT,
    PAIR_STRING_FLOAT,
    PAIR_FLOAT_FLOAT
};

class Data {
private:
    std::string name; //범례에 표시되는 값
    DataType type;
    void* data; //데이터 포인터
public:
    Data(std::string name, DataType type, void* data) : name(name), type(type), data(data) {}
    std::string getName() { return name; }
    DataType getType() { return type; }
    const void* getData(){ return data; }

};


class DataSet {
private:
    std::string name;
    std::vector<Data> datas;
public:
    DataSet() : name("new dataset") {}
    DataSet(std::string& name) : name(name) {}

    void addData(std::string name, std::vector<INT>* data){ datas.push_back(Data(name,DataType::SINGLE_INT, data)); }
    void addData(std::string name, std::vector<FLOAT>* data){ datas.push_back(Data(name,DataType::SINGLE_FLOAT, data)); }
    void addData(std::string name, std::vector<STRING>* data){ datas.push_back(Data(name,DataType::SINGLE_STRING, data)); }
    void addData(std::string name, std::vector<Pair<STRING,INT>>* data){ datas.push_back(Data(name,DataType::PAIR_STRING_INT, data)); }
    void addData(std::string name, std::vector<Pair<STRING,FLOAT>>* data){ datas.push_back(Data(name,DataType::PAIR_STRING_FLOAT, data)); }
    void addData(std::string name, std::vector<Pair<FLOAT,FLOAT>>* data){ datas.push_back(Data(name,DataType::PAIR_FLOAT_FLOAT, data)); }

    bool deleteData(std::string name);
    bool deleteData(unsigned int index);
    void deleteAll() { datas.clear(); }

    Data* getData(std::string name);
    Data* getData(unsigned int index);

    void printData(std::string name);
    void printData(unsigned int index);
    void printAll();
};

struct Gradation {
    Simd::Font font = Simd::Font(10);
    bool show = true;
    FLOAT min_value = -10;
    FLOAT max_value = 10;
    FLOAT major_interval = 2;
    FLOAT minor_interval = 1;
    FLOAT major_tick = (max_value-min_value)/major_interval;
    FLOAT minor_tick = (max_value-min_value)/minor_interval;
    int digit = 3;
    std::string label;
    Size size;
    Location location;
    View view_gradation;
    size_t margin = 20;// 좌우 마진
};

class Axis {
private:
    void* view_x_data;
    void* view_y_data;
protected:
    bool axis_changed = true;
    Gradation x_axis;
    Gradation y_axis;
    DataType type;
    inline Location origin_x_axis(size_t x, size_t y) {
        return Location(x + x_axis.margin, y);
    }
    inline Location origin_y_axis(size_t x, size_t y) {
        return Location(y_axis.size.width - x - 1, y_axis.size.height - y -1 + y_axis.margin);
    }
public:
    Axis(Size plot_size, Location plot_location) {
        x_axis.size = Size(plot_size.width, plot_size.height*0.15);
        y_axis.size = Size(plot_size.width*0.15,plot_size.height);
        x_axis.location = Location(plot_location.x - x_axis.margin, plot_location.y + plot_size.height);
        y_axis.location = Location(plot_location.x - y_axis.size.width, plot_location.y - y_axis.margin);

        view_x_data = SimdAllocate((x_axis.size.width+x_axis.margin*2)*x_axis.size.height*FORMAT,16);
        x_axis.view_gradation = View((x_axis.size.width+x_axis.margin*2),x_axis.size.height,(x_axis.size.width+x_axis.margin*2)*FORMAT,FORMAT,view_x_data);
        view_y_data = SimdAllocate(y_axis.size.width*(y_axis.size.height+y_axis.margin*2)*FORMAT,16);
        y_axis.view_gradation = View(y_axis.size.width,y_axis.size.height+y_axis.margin*2,y_axis.size.width*FORMAT,FORMAT,view_y_data);
    }
    ~Axis() {
        SimdFree(view_x_data);
        SimdFree(view_y_data);
    }
};

class Title {
private:
   void* m_title_view_data;
protected:
   bool title_changed = true;
   bool show_title = true;
   Size m_title_size;
   Simd::Font m_title_font = Simd::Font(20);
   Location m_title_location;
   std::string m_title;
   View m_title_view;
public:
   Title(Size size, Location location) : m_title_size(size), m_title_location(location), m_title("Plot") {
       m_title_view_data = SimdAllocate(m_title_size.width*m_title_size.height*FORMAT,16);
       m_title_view = View(m_title_size.width,m_title_size.height,m_title_size.width*FORMAT,FORMAT,m_title_view_data);
   }
   Title(Size size, Location location, std::string title) :m_title_size(size), m_title_location(location), m_title(title) {
       m_title_view_data = SimdAllocate(m_title_size.width*m_title_size.height*FORMAT,16);
       m_title_view = View(m_title_size.width,m_title_size.height,m_title_size.width*FORMAT,FORMAT,m_title_view_data);
   }
   ~Title() {
       if(m_title_view_data) { SimdFree(m_title_view_data); }
   }

   void setTitle(std::string title) { m_title = title; }
};

class Plot {
private:
    void* m_plot_view_data;
protected:
    bool plot_changed = true;
    bool show_grid = true;
    Size m_plot_size;
    Location m_plot_location;
    View m_plot_view;

    inline Location origin_plot(size_t x, size_t y) {
        return Location(x, m_plot_size.height - y -1);
    }
public:
    Plot(Size size, Location location) : m_plot_size(size), m_plot_location(location) {
       m_plot_view_data = SimdAllocate(m_plot_size.width*m_plot_size.height*FORMAT,16);
       m_plot_view = View(m_plot_size.width,m_plot_size.height,m_plot_size.width*FORMAT,FORMAT,m_plot_view_data);
    }
    ~Plot() {
        if(m_plot_view_data) {SimdFree(m_plot_view_data);}
    }

};

class rtplot : Title, Plot, Axis {
private:
    Size m_size;
    View m_view;
    void* m_view_data;
    DataSet m_dataset;

    Color color_axis = Color(0,0,0);
    Color color_axis_minor = Color(150,150,150);
    Color color_text = Color(0,0,0);
    Color color_grid = Color(200,200,200);
    //Location calLocation() { return Location(0,0); }

public:
    rtplot(size_t width, size_t height) :
        Title(Size(width*0.5,height*0.125),Location(width*0.25, height*0.025)),
        Plot(Size(width*0.8,height*0.65),Location(width*0.15,height*0.175)),
        Axis(m_plot_size,m_plot_location),
        m_size(width,height)
    {
        m_view_data = SimdAllocate(m_size.width*m_size.height*FORMAT, 16);
        m_view = View(m_size.width,m_size.height,m_size.width*FORMAT,FORMAT,m_view_data);
    }
    ~rtplot() {
        if(m_view_data) {SimdFree(m_view_data);}
    }

    const View* getView() { updatePlot(); return &m_view; }
    void init();
    void drawPlot();
    void updatePlot();
    void resize(size_t width, size_t height);
    void setDataSet(DataSet dataset);

};
}

#endif // RTPLOT_H
