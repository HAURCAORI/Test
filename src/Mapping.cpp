#include "Mapping.h"
#include "Calculate.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <exception>

#define cmap(arg) {#arg ,Command::arg}
#define firstSplitString(x) x.substr(1,x.find_first_of(' ') - 1) 
#define secondSplitString(x) x.substr(x.find_first_of(' ') + 1)

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

// { 1, 2, 3 } 형식의 데이터를 벡터로
std::vector<unsigned int> getDimension(std::string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());

    std::vector<unsigned int> vec;  
    if(str.front() == '{' && str.back() == '}') {
        str = str.substr(1,str.size()-2);
        std::vector<std::string> temp = split(str,",");
        vec.reserve(temp.size());
        try {
            for(auto it = temp.begin(); it != temp.end(); ++it) 
            {
                vec.push_back(stoi(*it));
            }
        } catch (std::exception e) {
            std::cout << "[ERROR]" << std::endl;
        }
    }
    return vec;
}




// 1. 모든 코드 라인을 읽고 정형화
// 2. 정형화된 코드 중 preprocessing 진행
// 3. 정형화된 코드 중 dataprocessing을  링킹을 위해 목록화
// 4. data 처리(linking, write)


enum class CommandType {
    NONE,
    PREPROCESSING,
    DATAPROCESSING
};

enum class Command {
    NONE,
    //preprocesing
    DIMENSION,
    DEFAULT,
    DEFINE,
    FUNCTION,
    //dataprocessing
    PAGE,
    DATA
};

static std::unordered_map<std::string,Command> const command_table = {
    cmap(DIMENSION), cmap(DEFAULT), cmap(DEFINE), cmap(FUNCTION), {"PAGE",Command::PAGE}, cmap(DATA)
};
Command matchingCommand(std::string str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    auto it = command_table.find(str);
    if (it != command_table.end()) {
        return it->second;
    } else { return Command::NONE; }
}

enum class Attribute {
    NONE,
    TYPE,
    SPECIFICITY,
    DIRECTION,
    THRESHOLD,
    WEIGHT,
    TARGET,
    OUT
};

struct AttributeData {
    Attribute attribute;
    std::vector<std::string> args;
};

struct codeLine {
    CommandType type;
    Command command;
    int group;
    std::string value;
};





bool Mapping() {
    std::cout << "===== Mapping =====" << std::endl;
    std::string address = (std::string)Path + "mapping";
	std::ifstream file(address);

    std::vector<codeLine> m_cl;

    bool error = false;

    // 1. 코드 정형화
    if (file.is_open())
    {
        std::string str; //get text line
        char token; //first text
        int group = 0; //group
        while (!file.eof())
        {
			std::getline(file, str);
            if(str.empty()) { continue; }
            codeLine tcl;

            token = str.front();
            switch(token) {
                case '#': {
                tcl.type = CommandType::PREPROCESSING;
                tcl.group = 0;
                tcl.value = secondSplitString(str);
                tcl.command = matchingCommand(firstSplitString(str));
                }
                break;

                case '$': {
                group++;
                tcl.type = CommandType::DATAPROCESSING;
                tcl.group = group;
                tcl.value = secondSplitString(str);
                Command tc = matchingCommand(firstSplitString(str));
                if(tc == Command::PAGE) {
                    tcl.command = tc;
                } else {
                    //[ERROR]
                }
                }

                break;

                case '[':
                tcl.type = CommandType::DATAPROCESSING;
                tcl.group = group;
                tcl.value = str;
                tcl.command = Command::DATA;
                break;

                case '/':
                if(str.at(1) == '/') { continue; }
                break;

                default:

                continue;
                break;
            }
            m_cl.push_back(tcl);
        }
    }
    file.close();

    if(error) {
        return false;
    }
    error = false;

    // 2. preprocessing
    bool valid = true;
    unsigned int m_dimension = 0;
    std::vector<unsigned int> m_dimsizes;
    m_dimsizes.reserve(5);

    for(auto it = m_cl.begin(); it != m_cl.end();) {
        if (it->type != CommandType::PREPROCESSING) { ++it; continue; }
        switch(it->command) {
            case Command::DIMENSION: {
                m_dimsizes = getDimension(it->value);
                if(m_dimsizes.empty()) {
                    error = true;
                    break;
                }
                m_dimension = m_dimsizes.size();
            }
            break;

            case Command::DEFAULT: {

            }
            break;
            
            case Command::DEFINE: {

            }
            break;

            case Command::FUNCTION: {

            }
            break;

            default:
            //[ERROR]
            break;

        }
        
        it = m_cl.erase(it);           
    }

    // 유효성 판단
    for(auto it = m_dimsizes.begin(); it != m_dimsizes.end(); ++it) {
        if(*it == 0 || *it > MAX_SECTOR) {
            valid = false;
            break;
        }
    }
    
    if(m_dimension == 0) {
        error = true;
        //[ERROR]
    }

    if(error || !valid) {
        return false;
    }


    //transform(str.begin(), str.end(),str.begin(), ::toupper);//대문자로 치환
    
    return true;
}

}