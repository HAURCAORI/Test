#pragma once

#include <stdio.h>
#include <fstream>
#include <vector>
#include <chrono>

#define SectorDimension 3
#define SectorSize 3

#define Path "resource/"

#define D1 0x01
#define D2 0x02
#define D3 0x04
#define D4 0x08
#define D5 0x10
#define D6 0x20
#define D7 0x40
#define D8 0x80

#define EPSILON 0.0001
#define MAX_SECTOR 1000000
#define Now() std::chrono::steady_clock::now()

typedef int PAGE;
typedef unsigned char BYTE;
typedef unsigned char BIT;
typedef unsigned char FLAG;
typedef unsigned char DIRECTION;
typedef std::chrono::steady_clock::time_point TIMESTAMP;

struct Signal
{
    float value;
    FLAG specificity;
    TIMESTAMP timestamp;
};

struct Neuron
{
    //Header
    BYTE type;
    // 1 bit : 패스
    // 2 bit : 억제성
    // 3 bit : 
    // 4 bit : 
    // 5 bit :
    // 6 bit : 타입(출력)
    // 7 bit : 타입(페이지 이동)
    // 8 bit : 활성화
    FLAG specificity; // 수용기에 대한 정보
    DIRECTION direction;
    TIMESTAMP timestamp;
    float value;
    //Value
    float threshold;
    float weight;
    Neuron* prev;
};

struct FileStruct {
    int fd;
    void* memory_area;
    size_t size_mapped;
    size_t size_header;
};

struct DataStruct {
    Neuron* data_area = nullptr;
    unsigned int dimension;
    std::vector<unsigned int> dimSizes;
};

struct PageFile
{
    PAGE id;
    
    FileStruct fs;
    DataStruct ds;

    PageFile(PAGE id) : id(id) {}
    PageFile(PAGE id, FileStruct fs, DataStruct ds) : id(id), fs(std::move(fs)), ds(std::move(ds)) {
        this->ds.data_area = reinterpret_cast<Neuron*>(((unsigned char*) this->fs.memory_area) + fs.size_header);
    }

    bool operator==(const PageFile &pf) const
    {
        if (this->id == pf.id)
            return true;
        return false;
    }
    struct HashFunction {
    size_t operator()(const PageFile &pf) const
    {
        return std::hash<int>()(pf.id);
    }
    };
};
