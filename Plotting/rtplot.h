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
inline void printVector(std::vector<T> vec) {
    std::cout << getType<T>() << ":\t";
    for(auto it = vec.begin(); it != vec.end(); ++it ) {
        std::cout << *it << '\t';
    }
    std::cout << std::endl;
}
template<typename T, typename U>
inline void printVector(std::vector<Pair<T,U>> vec) {
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
    int font_size = 10;
    bool show_gradation = true;
    FLOAT tick;
    FLOAT lim_min;
    FLOAT lim_max;
    std::string label;
    size_t width;
    size_t height;
    View view_gradation;
};

class Axis {
private:
    Gradation x_axis;
    Gradation y_axis;
    DataType type;
public:
    Axis(DataType type);
};

struct AxisData {
    Axis axis;
    DataSet dataset;
    AxisData(Axis axis, DataSet dataset) : axis(axis), dataset(dataset) {}
};

class Title {
private:
   bool show_title = true;
   std::string m_title;

   size_t m_width;
   size_t m_height;
   View m_view;
public:
   Title(size_t width, size_t height) : m_width(width), m_height(height), m_title("Plot") {
       m_view = View(width,height,FORMAT);
   }
   Title(size_t width, size_t height, std::string title) : m_width(width), m_height(height), m_title(title) {
       m_view = View(width,height,FORMAT);
   }

   void setTitle(std::string title) { m_title = title; }
};

class Plot {
private:
    size_t m_width;
    size_t m_height;
    View m_view;
public:
    Plot(size_t width, size_t height) : m_width(width), m_height(height) {
       m_view = View(width,height,FORMAT);
    }
};

class rtplot {
private:
    size_t m_width;
    size_t m_height;
    View m_view;


    Title m_title;
    AxisData m_axisdata;
    Plot m_plot;
public:
    rtplot(size_t width, size_t height);

    void init();

    void setDataSet(DataSet dataset) {
        m_axisdata = AxisData(Axis(DataType::PAIR_FLOAT_FLOAT), dataset);
        init();
    }

};
}

#endif // RTPLOT_H
