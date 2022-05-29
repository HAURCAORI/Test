#include "drawing.h"
#include <iostream>
#include <type_traits>
#include <plotstruct.h>
#ifndef RTPLOT_H
#define RTPLOT_H



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

struct Gradation {
    //global
    char id;
    bool show = true;
    std::string label;
    Size size;
    Location location;
    size_t margin = 20;// 좌우 마진
    //case number
    FLOAT min_value = -10;
    FLOAT max_value = 10;
    FLOAT imin_value = min_value;
    FLOAT imax_value = max_value;
    FLOAT major_interval = 2;
    FLOAT imajor_interval = major_interval;
    FLOAT minor_interval = 1;
    FLOAT iminor_interval = minor_interval;
    FLOAT major_tick = (max_value-min_value)/major_interval;
    FLOAT minor_tick = (max_value-min_value)/minor_interval;
    int digit = 3;
    //case string
    size_t data_number = 5;
};


class DataSet {
private:
    std::string name;
    std::vector<DataStruct> datas;
public:
    DataSet() : name("new dataset") {}
    DataSet(std::string& name) : name(name) {}
    DataSet(std::vector<DataStruct>& datas) : datas(datas) {}

    void addData(std::string name, std::vector<INT>* data){ datas.push_back(DataStruct(name,DataType::SINGLE_INT, data,data->size(),sizeof(INT))); }
    void addData(std::string name, std::vector<FLOAT>* data){ datas.push_back(DataStruct(name,DataType::SINGLE_FLOAT, data,data->size(),sizeof(FLOAT))); }
    void addData(std::string name, std::vector<STRING>* data){ datas.push_back(DataStruct(name,DataType::SINGLE_STRING, data,data->size(),sizeof(STRING))); }
    void addData(std::string name, std::vector<Pair<STRING,INT>>* data){ datas.push_back(DataStruct(name,DataType::PAIR_STRING_INT, data,data->size(),sizeof(Pair<STRING,INT>))); }
    void addData(std::string name, std::vector<Pair<STRING,FLOAT>>* data){ datas.push_back(DataStruct(name,DataType::PAIR_STRING_FLOAT, data,data->size(),sizeof(Pair<STRING,FLOAT>))); }
    void addData(std::string name, std::vector<Pair<FLOAT,FLOAT>>* data){ datas.push_back(DataStruct(name,DataType::PAIR_FLOAT_FLOAT, data,data->size(),sizeof(Pair<FLOAT,FLOAT>))); }

    bool deleteData(std::string name);
    bool deleteData(unsigned int index);
    void deleteAll() { datas.clear(); }

    void updateData(std::vector<DataStruct>* vec);

    std::vector<DataStruct>* getDatas() { return &datas; }
    DataStruct* getData(std::string name);
    DataStruct* getData(unsigned int index);
    size_t getSize() { return datas.size(); }

    void printData(std::string name);
    void printData(unsigned int index);
    void printAll();
};

class Axis {
private:
    void* view_x_data = nullptr;
    void* view_y_data = nullptr;
protected:
    Simd::Font font = Simd::Font(10);
    bool axis_changed = true;
    Gradation x_axis;
    Gradation y_axis;
    View view_x_axis;
    View view_y_axis;
    char principal_axis = 'y';

    inline Location origin_x_axis(int x, int y) {
        return Location(x + x_axis.margin, y);
    }
    inline Location origin_y_axis(int x, int y) {
        return Location(y_axis.size.width - x - 1, y_axis.size.height - y -1 - y_axis.margin);
    }
public:
    Axis() {}
    Axis(const Axis& rhs) : x_axis(rhs.x_axis), y_axis(rhs.y_axis) {
        if(view_x_data) SimdFree(view_x_data);
        if(view_y_data) SimdFree(view_y_data);
        view_x_data = SimdAllocate(x_axis.size.width*x_axis.size.height*FORMAT,16);
        view_x_axis = View(x_axis.size.width,x_axis.size.height,x_axis.size.width*FORMAT,FORMAT,view_x_data);
        view_y_data = SimdAllocate(y_axis.size.width*y_axis.size.height*FORMAT,16);
        view_y_axis = View(y_axis.size.width,y_axis.size.height,y_axis.size.width*FORMAT,FORMAT,view_y_data);
    }
    Axis& operator=(const Axis& rhs) {
        x_axis = rhs.x_axis;
        y_axis = rhs.y_axis;
        if(view_x_data) SimdFree(view_x_data);
        if(view_y_data) SimdFree(view_y_data);
        view_x_data = SimdAllocate(x_axis.size.width*x_axis.size.height*FORMAT,16);
        view_x_axis = View(x_axis.size.width,x_axis.size.height,x_axis.size.width*FORMAT,FORMAT,view_x_data);
        view_y_data = SimdAllocate(y_axis.size.width*y_axis.size.height*FORMAT,16);
        view_y_axis = View(y_axis.size.width,y_axis.size.height,y_axis.size.width*FORMAT,FORMAT,view_y_data);
        return *this;
    }
    Axis(Axis&& rhs) noexcept : x_axis(rhs.x_axis), y_axis(rhs.y_axis) {
        if(view_x_data) SimdFree(view_x_data);
        if(view_y_data) SimdFree(view_y_data);
        view_x_data = rhs.view_x_data;
        view_x_axis = std::move(rhs.view_x_axis);
        view_y_data = rhs.view_y_data;
        view_y_axis = std::move(rhs.view_y_axis);
        rhs.view_x_data = nullptr;
        rhs.view_y_data = nullptr;
    }

    Axis(Size plot_size, Location plot_location) {
        x_axis.id = 'x';
        y_axis.id = 'y';
        x_axis.size = Size(plot_size.width+x_axis.margin*2, plot_size.height*0.15);
        y_axis.size = Size(plot_size.width*0.15,plot_size.height+y_axis.margin*2);
        x_axis.location = Location(plot_location.x - x_axis.margin, plot_location.y + plot_size.height);
        y_axis.location = Location(plot_location.x - y_axis.size.width, plot_location.y - y_axis.margin);

        view_x_data = SimdAllocate(x_axis.size.width*x_axis.size.height*FORMAT,16);
        view_x_axis = View(x_axis.size.width,x_axis.size.height,x_axis.size.width*FORMAT,FORMAT,view_x_data);
        view_y_data = SimdAllocate(y_axis.size.width*y_axis.size.height*FORMAT,16);
        view_y_axis = View(y_axis.size.width,y_axis.size.height,y_axis.size.width*FORMAT,FORMAT,view_y_data);
    }
    ~Axis() {
        if(view_x_data) SimdFree(view_x_data);
        if(view_y_data) SimdFree(view_y_data);
    }

    Gradation* xAxis() { return &x_axis; }
    Gradation* yAxis() { return &y_axis; }

    Size xAxisSize() { return x_axis.size; }
    Location xAxisLocation() { return x_axis.location; }
    Size yAxisSize() { return y_axis.size; }
    Location yAxisLocation() { return y_axis.location; }
};

class Title {
private:
   void* m_title_view_data = nullptr;
protected:
   bool title_changed = true;
   bool show_title = true;
   Size m_title_size;
   Simd::Font m_title_font = Simd::Font(20);
   Location m_title_location;
   std::string m_title;
   View m_title_view;
public:
   Title() {}
   Title(const Title& rhs) : m_title_size(rhs.m_title_size), m_title_location(rhs.m_title_location), m_title(rhs.m_title) {
       if(m_title_view_data) { SimdFree(m_title_view_data); }
       m_title_view_data = SimdAllocate(m_title_size.width*m_title_size.height*FORMAT,16);
       m_title_view = View(m_title_size.width,m_title_size.height,m_title_size.width*FORMAT,FORMAT,m_title_view_data);
   }
   Title& operator=(const Title& rhs) {
       m_title_size = rhs.m_title_size;
       m_title_location = rhs.m_title_location;
       m_title = rhs.m_title;
       if(m_title_view_data) { SimdFree(m_title_view_data); }
       m_title_view_data = SimdAllocate(m_title_size.width*m_title_size.height*FORMAT,16);
       m_title_view = View(m_title_size.width,m_title_size.height,m_title_size.width*FORMAT,FORMAT,m_title_view_data);
       return *this;
   }
   Title(Title&& rhs) noexcept : m_title_size(rhs.m_title_size), m_title_location(rhs.m_title_location), m_title(rhs.m_title) {
       if(m_title_view_data) { SimdFree(m_title_view_data); }
       m_title_view_data = rhs.m_title_view_data;
       m_title_view = std::move(rhs.m_title_view);
       rhs.m_title_view_data = nullptr;
   }

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
    void* m_plot_view_data = nullptr;
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
    Plot() {}
    Plot(const Plot& rhs) : m_plot_size(rhs.m_plot_size), m_plot_location(rhs.m_plot_location) {
        if(m_plot_view_data) { SimdFree(m_plot_view_data); }
        m_plot_view_data = SimdAllocate(m_plot_size.width*m_plot_size.height*FORMAT,16);
        m_plot_view = View(m_plot_size.width,m_plot_size.height,m_plot_size.width*FORMAT,FORMAT,m_plot_view_data);
    }
    Plot& operator=(const Plot& rhs) {
        m_plot_size = rhs.m_plot_size;
        m_plot_location = rhs.m_plot_location;
        if(m_plot_view_data) { SimdFree(m_plot_view_data); }
        m_plot_view_data = SimdAllocate(m_plot_size.width*m_plot_size.height*FORMAT,16);
        m_plot_view = View(m_plot_size.width,m_plot_size.height,m_plot_size.width*FORMAT,FORMAT,m_plot_view_data);
        return *this;
    }
    Plot(Plot&& rhs) noexcept : m_plot_size(rhs.m_plot_size), m_plot_location(rhs.m_plot_location) {
        if(m_plot_view_data) { SimdFree(m_plot_view_data); }
        m_plot_view_data = rhs.m_plot_view_data;
        m_plot_view = std::move(rhs.m_plot_view);
        rhs.m_plot_view_data = nullptr;
    }
    Plot(Size size, Location location) : m_plot_size(size), m_plot_location(location) {
       m_plot_view_data = SimdAllocate(m_plot_size.width*m_plot_size.height*FORMAT,16);
       m_plot_view = View(m_plot_size.width,m_plot_size.height,m_plot_size.width*FORMAT,FORMAT,m_plot_view_data);
    }
    ~Plot() {
        if(m_plot_view_data) {SimdFree(m_plot_view_data);}
    }

    Size plotSize() { return m_plot_size; }
    Location plotLocation() { return m_plot_location; }
};

class rtplot : public Title, public Plot, public Axis {
private:
    Size m_size;
    View m_view;
    void* m_view_data = nullptr;
    DataSet m_dataset;
    DataType type = DataType::PAIR_FLOAT_FLOAT;

    Color color_axis = Color(0,0,0);
    Color color_axis_minor = Color(150,150,150);
    Color color_text = Color(0,0,0);
    Color color_grid = Color(200,200,200);
    void drawData();
public:
    rtplot() {}
    rtplot(const rtplot& rhs) : Title(rhs), Plot(rhs), Axis(rhs), m_size(rhs.m_size), m_dataset(rhs.m_dataset), type(rhs.type) {
        if(m_view_data) { SimdFree(m_view_data); }
        m_view_data = SimdAllocate(m_size.width*m_size.height*FORMAT, 16);
        m_view = View(m_size.width,m_size.height,m_size.width*FORMAT,FORMAT,m_view_data);
        init();
    }
    rtplot& operator=(const rtplot& rhs){
        m_size = rhs.m_size;
        m_dataset = rhs.m_dataset;
        type = rhs.type;
        if(m_view_data) { SimdFree(m_view_data); }
        m_view_data = SimdAllocate(m_size.width*m_size.height*FORMAT, 16);
        m_view = View(m_size.width,m_size.height,m_size.width*FORMAT,FORMAT,m_view_data);
        Title::operator=(rhs);
        Plot::operator=(rhs);
        Axis::operator=(rhs);
        init();
        return *this;
    }
    rtplot(rtplot&& rhs) noexcept : Title(std::move(rhs)), Plot(std::move(rhs)), Axis(std::move(rhs)), m_size(rhs.m_size), m_dataset(rhs.m_dataset), type(rhs.type){
        std::cout <<"move";
        if(m_view_data) { SimdFree(m_view_data); }
        m_view_data = rhs.m_view_data;
        m_view = std::move(rhs.m_view);
        rhs.m_view_data = nullptr;
    }

    rtplot(size_t width, size_t height) :
        Title(Size(width*0.5,height*0.125),Location(width*0.25, height*0.025)),
        Plot(Size(width*0.8,height*0.65),Location(width*0.15,height*0.175)),
        Axis(m_plot_size,m_plot_location),
        m_size(width,height)
    {
        m_view_data = SimdAllocate(m_size.width*m_size.height*FORMAT, 16);
        m_view = View(m_size.width,m_size.height,m_size.width*FORMAT,FORMAT,m_view_data);
        init();
    }
    ~rtplot() {
        if(m_view_data) {SimdFree(m_view_data);}
    }

    const View* getView() { updatePlot(); return &m_view; }
    void init();
    void drawPlot();
    void updatePlot();
    void movePlot(int x, int y);
    void moveOrigin(Gradation *axis);
    void scalePlot(Gradation* axis, FLOAT pivet, int delta);
    void scaleOrigin(Gradation* axis, FLOAT pivet);
    void resize(size_t width, size_t height);
    void setDataSet(DataSet dataset, DataType type);
    DataSet* getDataSet() { return &m_dataset; }
    void updateDataSet(std::vector<DataStruct>* vec) { m_dataset.updateData(vec); }
};
}

#endif // RTPLOT_H
