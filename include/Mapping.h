#pragma once
#include "NStruct.h"

namespace Mapping {
Neuron createNeuron();
Neuron createNeuron(DIRECTION direction);
Neuron createNeuron(DIRECTION direction, float threshold);
Neuron createNeuron(DIRECTION direction, float threshold, float weight);

void writeDataStruct(FILE* stream, long pos, Neuron&& sender);
bool Mapping();

}