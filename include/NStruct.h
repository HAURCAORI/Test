#pragma once
#include <stdio.h>
#define SectorLayer 5
#define SectorSize 10

#define Path "resource/"

#define EPSILON 0.0001

typedef unsigned char BYTE;
typedef unsigned char BIT;
typedef unsigned char FLAG;
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
    TIMESTAMP timestamp;
    float value;
    //Value
    float threshold;
    float weight;
};