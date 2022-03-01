#include "main.h"
#include "DataProcess.h"
#include "Mapping.h"

int main(){
    Neuron neuron[SectorLayer][SectorSize][SectorSize];
    neuron[0][1][1] = CreateNeuron(0X10);
    neuron[1][1][1] = CreateNeuron(0X0F);
    Signal signal = {1.0f, 0xff};
    Load(neuron,0,1,1, &signal);
    return 0;
}