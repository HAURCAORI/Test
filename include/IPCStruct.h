#pragma once
#include <string>
#include <map>
#include <vector>
#ifndef _IPC_STRUCT_
#define _IPC_STRUCT_

typedef int INT;
typedef double FLOAT;
typedef std::string STRING;

#define IPC_STRUCT_STRING_SIZE 20

namespace DataIO {
namespace IPCStruct {

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

enum class IPCDataType :char {
    SINGLE_INT = 0x01,
    SINGLE_FLOAT = 0x02,
    SINGLE_STRING = 0x03,
    PAIR_STRING_INT = 0x04,
    PAIR_STRING_FLOAT = 0x05,
    PAIR_FLOAT_FLOAT = 0x06
};

class IPCDataStruct {
private:
    IPCDataType type;
    std::string name; 
    int m_size;
    int type_size;
    void* data = nullptr; //데이터 포인터
public:
    IPCDataStruct(std::string name, IPCDataType type, void* data, size_t size, int type_size) : type(type), name(name), m_size(size), type_size(type_size),data(data) {}
    std::string getName() { return name; }
    IPCDataType getType() { return type; }
    void* getData(){ return data; }
    int size() { return m_size; }
    int typeSize() { return type_size; }
    int getIPCDataSize() { return (1 + IPC_STRUCT_STRING_SIZE + 4 + 1 + m_size * type_size); }
};

class VectorContainer {
private:
    std::map<std::string, std::vector<FLOAT>> m_vec_f;
public:
    VectorContainer() {}
    std::vector<FLOAT>* add(std::string id, std::vector<FLOAT>&& vec) { return &(m_vec_f.emplace(std::make_pair(id,vec)).first->second);}
    
    std::vector<FLOAT>* get(std::string id) { return &(m_vec_f.find(id)->second); }
};

}
}
#endif