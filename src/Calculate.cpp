#include "Calculate.h"
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

float threshold_mean = 0.6;
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


std::vector<std::string> split(std::string str, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> ret;

    while ((pos_end = str.find (delimiter, pos_start)) != std::string::npos) {
        token = str.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        ret.push_back (token);
    }
    ret.push_back (str.substr (pos_start));
    return ret;
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}