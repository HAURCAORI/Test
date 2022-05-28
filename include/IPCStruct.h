#pragma once
#include <iostream>

#ifndef _IPC_STRUCT_H
#define _IPC_STRUCT_H
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

namespace DataIO {
namespace IPCStruct {
enum class DataType : char {
    SINGLE_INT = 0x01,
    SINGLE_FLOAT = 0x02,
    SINGLE_STRING = 0x03,
    PAIR_STRING_INT = 0x04,
    PAIR_STRING_FLOAT = 0x05,
    PAIR_FLOAT_FLOAT = 0x06
};

class DataStruct {
private:
    std::string name;
    DataType type;
    void* data; //데이터 포인터
    size_t m_size;
    int type_size;
public:
    DataStruct(std::string name, DataType type, void* data, size_t size, int type_size) : name(name), type(type), data(data), m_size(size), type_size(type_size) {}
    std::string getName() { return name; }
    DataType getType() { return type; }
    const void* getData(){ return data; }
    size_t size() { return m_size; }
    int typeSize() { return type_size; }
};
}
}
#endif