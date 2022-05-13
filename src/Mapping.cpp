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
        std::string order;
        char token;
        while (!file.eof())
        {
            order.clear();
            
			std::getline(file, str);
            if(str.empty()) { continue; }

            token = str.front();
            switch(token) {
                case '#': //dimension
                order = str.substr(1,str.find_first_of(' '));
                break;

                case '$':
                order = str.substr(1,str.find_first_of(' '));
                break;

                case '[':
                order = "data";
                break;

                case '/':
                if(str.at(1) == '/') { continue; }
                break;

                default:
                order = "";
                break;
            }
        }
        //map을 만들어서 page-sector 데이터 저장 후 있는지 판단
    }
    //transform(str.begin(), str.end(),str.begin(), ::toupper);//대문자로 치환
    
    return true;
}

}