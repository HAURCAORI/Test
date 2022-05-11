#pragma once
#include "NStruct.h"
#include <fstream>
#include <unordered_set>


#define PATH "resource/"
#define DefaultReserve 10

namespace DataIO
{
class IOManager {
    private:
        std::unordered_set<PageFile,PageFile::HashFunction> pagefiles;
    public:
        IOManager();
        ~IOManager();
        bool createPage(PAGE id);
        bool deletePage(PAGE id);
        bool loadPage(PAGE id);
        bool unloadPage(PAGE id);
        bool unloadPage(PageFile pf);
        void printPage();

        inline void* getPointer(PAGE id) { return pagefiles.find(id)->fs.memory_area; }
        inline void* getPointer(PageFile pf) { return pf.fs.memory_area; }
        inline PageFile getPageFile(PAGE id) { return *pagefiles.find(id); }
        const DataStruct* getDataStruct(PAGE id) { return &((*pagefiles.find(id)).ds); }
};

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

}