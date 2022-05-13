#include "Calculate.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>

#define RANGE 0.5

inline BIT getBit(const BYTE byte, const int n)
{
    return (byte >> (7 - n)) & 1;
}

// random device class instance, source of 'true' randomness for initializing random seed
std::random_device rd; 
// Mersenne twister PRNG, initialized with seed from previous random device instance
std::mt19937 gen(rd()); 

float threshold_mean = 0.2;
float threshold_stddev = 0.1;
float weight_mean = 0.8;
float weight_stddev = 0.2;


float random_float()
{
    std::srand(std::time(nullptr));
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/RANGE));
}



float random_threshold()
{
    std::normal_distribution<float> dist(threshold_mean, threshold_stddev);
    return dist(gen);
}

float random_weight()
{
    std::normal_distribution<float> dist(weight_mean, weight_stddev);
    return dist(gen);
}

