#include "Mapping.h"

Neuron CreateNeuron(DIRECTION direction){
    Neuron temp = {0x00, 0xff,direction, 0.0f, 0.0f, random_threshold(),random_weight()};
    return temp;
}
Neuron CreateNeuron(DIRECTION direction, float threshold){
    Neuron temp = {0x00, 0xff,direction, 0.0f, 0.0f, threshold,random_weight()};
    return temp;
}
Neuron CreateNeuron(DIRECTION direction, float threshold, float weight){
    Neuron temp = {0x00, 0xff,direction, 0.0f, 0.0f, threshold, weight};
    return temp;
}