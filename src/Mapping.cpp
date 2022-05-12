#include "Mapping.h"
#include "Calculate.h"
#include <fstream>

namespace Mapping {
Neuron createNeuron(){
    Neuron temp = {0x00, 0xff, 0x00, 0.0f, 0.0f, random_threshold(),random_weight()};
    return temp;
}
Neuron createNeuron(DIRECTION direction){
    Neuron temp = {0x00, 0xff,direction, 0.0f, 0.0f, random_threshold(),random_weight()};
    return temp;
}
Neuron createNeuron(DIRECTION direction, float threshold){
    Neuron temp = {0x00, 0xff,direction, 0.0f, 0.0f, threshold,random_weight()};
    return temp;
}
Neuron createNeuron(DIRECTION direction, float threshold, float weight){
    Neuron temp = {0x00, 0xff,direction, 0.0f, 0.0f, threshold, weight};
    return temp;
}

void writeDataStruct(FILE* stream, long pos, Neuron&& sender) {
    fseek(stream, pos, SEEK_SET);
    fwrite(&sender, sizeof(Neuron), 1, stream);
}

bool Mapping() {
    std::string address = (std::string)Path + "mapping";
	std::ifstream file(address);
    if (file.is_open())
    {
        std::string str;
        while (!file.eof())
        {
			//pos = file.tellg();
			std::getline(file, str);
        }
    }
    //transform(str.begin(), str.end(),str.begin(), ::toupper);//대문자로 치환
    
    return true;
}

}