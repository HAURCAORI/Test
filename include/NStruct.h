#pragma once

#include <stdio.h>
#include <fstream>
#include <vector>

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

typedef int PAGE;
typedef unsigned char BYTE;
typedef unsigned char BIT;
typedef unsigned char FLAG;
typedef unsigned char DIRECTION;
typedef float TIMESTAMP;

struct Signal
{
    float value;
    FLAG specificity;
};

struct Neuron
{
    //Header
    BYTE type;
    // 1 bit :
    // 2 bit :
    // 3 bit :
    // 4 bit :
    // 5 bit :
    // 6 bit :
    // 7 bit :
    // 8 bit : 전달로
    FLAG specificity; // 수용기에 대한 정보
    DIRECTION direction;
    TIMESTAMP timestamp;
    float value;
    //Value
    float threshold;
    float weight;
};

struct PageFile
{
    PAGE id;
    int fd;
    void* memory_area;
    size_t size_mapped;
    
    int dimension;
    std::vector<int> dimSizes;

    PageFile(PAGE id) : id(id) {}
    PageFile(PAGE id, int fd, void* memory_area, size_t length_to_map, int dimension, std::vector<int> dimSizes) : id(id), fd(fd), memory_area(std::move(memory_area)), size_mapped(length_to_map), dimension(dimension), dimSizes(std::move(dimSizes)) {}

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

