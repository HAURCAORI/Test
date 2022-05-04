#pragma once
#include "NStruct.h"
#include <fstream>

#define PATH "resource/"

template<typename T>
void saveData(std::string file_name, T data) {
    std::string address = (std::string)PATH + file_name;
    FILE *stream = fopen(address.c_str(), "wb");
    if(!stream) { return; }
    fwrite(&data,sizeof(T),1,stream);
    fclose(stream);
}

template<typename T>
T readData(std::string file_name) {
    std::string address = (std::string)PATH + file_name;
    FILE *stream = fopen(address.c_str(), "rb");
    if(!stream) { return 0; }
    T data;
    fread(&data,sizeof(T),1,stream);
    fclose(stream);
    return data;
}
