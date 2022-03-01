#pragma once
#include "DataProcess.h"

Neuron CreateNeuron(DIRECTION direction);
Neuron CreateNeuron(DIRECTION direction, float threshold);
Neuron CreateNeuron(DIRECTION direction, float threshold, float weight);