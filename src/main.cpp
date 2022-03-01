#include "main.h"
#include "DataProcess.h"
#include "Mapping.h"

int main(){
    Neuron neuron[SectorLayer][SectorSize][SectorSize];
    neuron[0][1][1] = CreateNeuron(0X10);
    Signal signal = {1.0f, 0xff};
    Load(neuron, &signal);
    return 0;
}