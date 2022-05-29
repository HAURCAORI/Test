#include "../include/IPCStruct.h"
#ifndef PLOTSTRUCT_H
#define PLOTSTRUCT_H

typedef int INT;
typedef double FLOAT;
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

namespace rtplot {


enum class DataType :char {
    SINGLE_INT = 0x01,
    SINGLE_FLOAT = 0x02,
    SINGLE_STRING = 0x03,
    PAIR_STRING_INT = 0x04,
    PAIR_STRING_FLOAT = 0x05,
    PAIR_FLOAT_FLOAT = 0x06
};

class DataStruct {
private:
    DataType type;
    std::string name; //범례에 표시되는 값
    int m_size;
    int type_size;
    void* data = nullptr; //데이터 포인터
public:
    DataStruct(std::string name, DataType type, void* data, size_t size, int type_size) : type(type), name(name), m_size(size), type_size(type_size),data(data)  {}
    DataStruct(DataIO::IPCStruct::IPCDataStruct& ipc) : type((DataType)ipc.getType()), name(ipc.getName()), m_size(ipc.size()), type_size(ipc.typeSize()),data(ipc.getData())  {}
    std::string getName() { return name; }
    DataType getType() { return type; }
    void* getData(){ return data; }
    void setData(void* src) { data = src; }
    int size() { return m_size; }
    int typeSize() { return type_size; }
};

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

}

#endif // PLOTSTRUCT_H
