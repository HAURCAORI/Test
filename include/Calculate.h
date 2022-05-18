#pragma once
#include "NStruct.h"
#include <vector>
#include <string>
#include <iostream>

inline BIT getBit(const BYTE byte, const int n);

float random_float();
float random_threshold();
float random_weight();

std::vector<std::string> split(std::string str, std::string delimiter);

inline bool isInteger(const std::string & str)
{
   if(str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) return false;

   char * p;
   strtol(str.c_str(), &p, 10);

   return (*p == 0);
}

template<typename T>
inline void printVector(std::vector<T> vec) {
    for(auto it = vec.begin(); it != vec.end(); ++it ) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}

void printBits(size_t const size, void const * const ptr);