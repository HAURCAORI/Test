#include "rtplot.h"

namespace rtplot {
bool DataSet::deleteData(std::string name) {
    for(auto it = datas.begin(); it != datas.end();) {
        if(it->name == name) { it = datas.erase(it); return true; }
        it++;
    }
    return false;
}
bool DataSet::deleteData(unsigned int index) {
    if(datas.size() > index ) {
        datas.erase(std::next(datas.begin(), index));
        return true;
    }
    return false;
}

Data* DataSet::getData(std::string name) {
    for(auto it = datas.begin(); it != datas.end(); ++it) {
        if(it->name == name) { return &(*it); }
    }
    return nullptr;
}
Data* DataSet::getData(unsigned int index) {
    if(datas.size() > index ) {
        return &datas[index];
    }
    return nullptr;
}

void printDataByType(const Data& data) {
    if(data.type == DataType::SINGLE_INT) {
        auto d = static_cast<std::vector<INT>*>(data.values);
        printVector(*d);
    } else if(data.type == DataType::SINGLE_FLOAT) {
        auto d = static_cast<std::vector<FLOAT>*>(data.values);
        printVector(*d);
    } else if(data.type == DataType::SINGLE_STRING) {
        auto d = static_cast<std::vector<STRING>*>(data.values);
        printVector(*d);
    } else if(data.type == DataType::PAIR_FLOAT_FLOAT) {
        auto d = static_cast<std::vector<Pair<FLOAT,FLOAT>>*>(data.values);
        printVector<FLOAT,FLOAT>(*d);
    } else if(data.type == DataType::PAIR_STRING_INT) {
        auto d = static_cast<std::vector<Pair<STRING,INT>>*>(data.values);
        printVector<STRING,INT>(*d);
    } else if(data.type == DataType::PAIR_STRING_FLOAT) {
        auto d = static_cast<std::vector<Pair<STRING,FLOAT>>*>(data.values);
        printVector<STRING,FLOAT>(*d);
    }
}

void DataSet::printData(std::string name) {
    for(auto it = datas.begin(); it != datas.end();++it) {
        if(it->name == name) {
            std::cout << "DataSet ['" << name << "'] :" << std::endl;
            printDataByType(*it);
        }
    }
}
void DataSet::printData(unsigned int index) {
    if(datas.size() <= index) { return; }
    std::cout << "DataSet ['" << datas[index].name << "'] :" << std::endl;
    printDataByType(datas[index]);
}
void DataSet::printAll() {
    for(auto it = datas.begin(); it != datas.end();++it) {
        std::cout << "DataSet ['" << (*it).name << "'] :" << std::endl;
        printDataByType(*it);
    }
}
}
