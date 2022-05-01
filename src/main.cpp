#include "main.h"
#include "DataProcess.h"
#include "Mapping.h"

int main(){
    Neuron neuron[SectorLayer][SectorSize][SectorSize] = {0};
    
    neuron[0][1][1] = CreateNeuron(0X10);
    neuron[1][1][1] = CreateNeuron(D5 | 0X03);
    neuron[1][1][2] = CreateNeuron(D5);
    neuron[1][1][0] = CreateNeuron(D5);
    neuron[2][1][2] = CreateNeuron(D2);
    neuron[2][1][0] = CreateNeuron(D1);

    Signal signal = {1.0f, 0xff};
    //for(int i = 0; i < 10; i++) {
        TLoad(neuron,0,1,1, &signal);
    //}
    return 0;
}