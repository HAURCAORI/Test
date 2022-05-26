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

struct Gradation {
    bool show = true;
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
    std::string label;
    Size size;
    Location location;
    size_t margin = 20;// 좌우 마진
};

}

#endif // PLOTSTRUCT_H
