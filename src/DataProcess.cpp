#include "DataProcess.h"
#include <math.h>
#include <chrono>
#define BEGIN_CHRONO std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#define END_CHRONO std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;


bool Load(Neuron (*target)[SectorSize][SectorSize], Signal* signal){
    
    return true;
}