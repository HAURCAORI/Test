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

struct Data {
    std::string name; //범례에 표시되는 값
    DataType type;
    void* values; //데이터 포인터
};

class DataSet {
private:
    std::string name;
    std::vector<Data> datas;
public:
    DataSet() : name("new dataset") {}
    DataSet(std::string& name) : name(name) {}

    void addData(std::string name, std::vector<INT>* data){ datas.push_back({name,DataType::SINGLE_INT, data}); }
    void addData(std::string name, std::vector<FLOAT>* data){ datas.push_back({name,DataType::SINGLE_FLOAT, data}); }
    void addData(std::string name, std::vector<STRING>* data){ datas.push_back({name,DataType::SINGLE_STRING, data}); }
    void addData(std::string name, std::vector<Pair<STRING,INT>>* data){ datas.push_back({name,DataType::PAIR_STRING_INT, data}); }
    void addData(std::string name, std::vector<Pair<STRING,FLOAT>>* data){ datas.push_back({name,DataType::PAIR_STRING_FLOAT, data}); }
    void addData(std::string name, std::vector<Pair<FLOAT,FLOAT>>* data){ datas.push_back({name,DataType::PAIR_FLOAT_FLOAT, data}); }

    bool deleteData(std::string name);
    bool deleteData(unsigned int index);
    void deleteAll() { datas.clear(); }

    Data* getData(std::string name);
    Data* getData(unsigned int index);

    void printData(std::string name);
    void printData(unsigned int index);
    void printAll();
};


class Axis {
private:
    FLOAT value_max;
    FLOAT value_min;


public:
};

class rtplot {
private:

public:

};
}

#endif // RTPLOT_H
