#pragma once
#include "NStruct.h"

namespace Mapping {
Neuron createNeuron();
Neuron createNeuron(DIRECTION direction);
Neuron createNeuron(DIRECTION direction, float threshold);
Neuron createNeuron(DIRECTION direction, float threshold, float weight);

std::vector<std::vector<unsigned int>> getD(std::vector<std::string>& strs, const std::vector<unsigned int>& dimsizes, std::vector<unsigned int> vec);

void writeDataStruct(FILE* stream, long pos, Neuron&& sender);
bool Mapping();

}