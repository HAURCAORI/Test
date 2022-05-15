#pragma once
#include "NStruct.h"

#include <chrono>
#define BEGIN_CHRONO std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#define END_CHRONO std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;

namespace Mapping {
Neuron createNeuron();
Neuron createNeuron(DIRECTION direction);
Neuron createNeuron(DIRECTION direction, float threshold);
Neuron createNeuron(DIRECTION direction, float threshold, float weight);

std::vector<std::vector<unsigned int>> getDimensions(std::string& strs, const std::vector<unsigned int>& dimsizes);

void writeDataStruct(FILE* stream, long pos, Neuron&& sender);
bool Mapping();


}