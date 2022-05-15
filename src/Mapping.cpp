#include "Mapping.h"
#include "Calculate.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <exception>

#define cmap(arg) {#arg ,Command::arg}
#define firstSplitString(x) x.substr(1,x.find_first_of(' ') - 1) 
#define secondSplitString(x) x.substr(x.find_first_of(' ') + 1)

struct MappingPoint {
    PAGE id;
    std::vector<unsigned int> point;

    bool operator==(const MappingPoint &mp) const
    {
        if (this->id == mp.id)
            if(this->point == mp.point)
                return true;
        return false;
    }

    struct HashFunction {
    size_t operator()(const MappingPoint &mp) const
    {
        std::hash<unsigned int> hasher;
        size_t seed = mp.id;
        for (unsigned int i : mp.point) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
    };
};

namespace Mapping {

enum class ErrorType {
    GENERAL,
    PREPROCESSING,
    PROCESSING,
    SYNTAX
};

inline void errorMsg(std::string&& e, int line = 0, ErrorType type = ErrorType::GENERAL) {
    switch(type) {
        case ErrorType::GENERAL:
            std::cerr << "[Error]Mapping: " << e << std::endl;
        break;
        case ErrorType::PREPROCESSING:
            std::cerr << "[Error]Preprocessing:" << line << ": " << e << std::endl;
        break;
        case ErrorType::PROCESSING:
            std::cerr << "[Error]Processing:" << line << ": " << e << std::endl;
        break;
        case ErrorType::SYNTAX:
            std::cerr << "[Error]Syntax:" << line << ": " << e << std::endl;
        break;
    }
}

inline void warningMsg(std::string&& e, int line = 0, ErrorType type = ErrorType::GENERAL) {
    switch(type) {
        case ErrorType::GENERAL:
            std::cerr << "[Warning]Mapping: " << e << std::endl;
        break;
        case ErrorType::PREPROCESSING:
            std::cerr << "[Warning]Preprocessing:" << line << ": " << e << std::endl;
        break;
        case ErrorType::PROCESSING:
            std::cerr << "[Warning]Processing:" << line << ": " << e << std::endl;
        break;
        case ErrorType::SYNTAX:
            std::cerr << "[Warning]Syntax:" << line << ": " << e << std::endl;
        break;
    }
}

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
    if((str.front() == '{' && str.back() == '}') || (str.front() == '[' && str.back() == ']')) {
        str = str.substr(1,str.size()-2);
        std::vector<std::string> temp = split(str,",");
        vec.reserve(temp.size());
        try {
            for(auto it = temp.begin(); it != temp.end(); ++it) 
            {
                vec.push_back(stoi(*it));
            }
        } catch (std::exception e) {
            errorMsg("'{" + str + "}' Can't Convert to Vector.");
        }
    }
    return vec;
}

std::vector<std::vector<unsigned int>> getDimensions(std::string str, std::vector<unsigned int> dimsizes) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());

    std::vector<std::vector<unsigned int>> vecs; 
    if((str.front() == '{' && str.back() == '}') || (str.front() == '[' && str.back() == ']')) {
        str = str.substr(1,str.size()-2);
        std::vector<std::string> temp = split(str,",");
        if(temp.size() != dimsizes.size()) {
            errorMsg("'{" + str + "}' size shold be " + std::to_string(dimsizes.size()) + ".");
            vecs.clear();
            return vecs;
        }

        int index = 0;
        for(auto it = temp.begin(); it != temp.end(); ++it, ++index) {
            std::vector<unsigned int> vec;
            if(isInteger(*it)) {
                vec.push_back(stoi(*it));
            }else {
                if((*it).find('*')) {
                    for(unsigned int i = 0; i < dimsizes[index]; i++) {
                        vec.push_back

                    }
                } else if ((*it).find('~')) {

                }
            }
        }
    }
    return vecs
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
    int line;
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
        int line = 0;
        while (!file.eof())
        {
			std::getline(file, str);
            ++line;
            if(str.empty()) { continue; }
            codeLine tcl;
            tcl.line = line;
            token = str.front();
            switch(token) {
                case '#': {
                tcl.type = CommandType::PREPROCESSING;
                tcl.group = 0;
                tcl.value = secondSplitString(str);
                tcl.command = matchingCommand(firstSplitString(str));
                m_cl.push_back(tcl);
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
                m_cl.push_back(tcl);
                }

                break;

                case '[':
                tcl.type = CommandType::DATAPROCESSING;
                tcl.group = group;
                tcl.value = str;
                tcl.command = Command::DATA;
                m_cl.push_back(tcl);
                break;

                case '/':
                if(str.at(1) == '/') { continue; }
                break;

                default:
                warningMsg("Can't encode line.", line, ErrorType::SYNTAX);
                continue;
                break;
            }
            
        }
    }
    else {
        errorMsg("Can't open 'mapping' file.");
    }
    file.close();

    if(error) {
         errorMsg("Regularizing Fail.");
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
                    errorMsg("Dimension is Empty.", it->line, ErrorType::PREPROCESSING);
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
            warningMsg("Undefined Command.", it->line, ErrorType::PREPROCESSING);
            break;

        }
        it = m_cl.erase(it);           
    }

    // 유효성 판단
    for(auto it = m_dimsizes.begin(); it != m_dimsizes.end(); ++it) {
        if(*it == 0) {
            valid = false;
            errorMsg("Dimension is 0.");
            break;
        }
        if(*it > MAX_SECTOR) {
            valid = false;
            errorMsg("Dimension Out of Range.");
            break;
        }
    }
    
    if(m_dimension == 0) {
        error = true;
        errorMsg("Must define '#DIMENSION'.");
    }

    if(error || !valid) {
        errorMsg("Preprocessing Fail.");
        return false;
    }
    error = false;

    // 3. linking
    std::unordered_set<MappingPoint, MappingPoint::HashFunction> m_points;
    std::string current_page;
    for(auto it = m_cl.begin(); it != m_cl.end();++it) {
        if(it->type != CommandType::DATAPROCESSING) { continue; }
        if(it->command == Command::PAGE) {
            if(!isInteger(it->value)) {
                error = true;
                errorMsg("PAGE shold be Integer.", it->line, ErrorType::PROCESSING);
                break;
            }
            current_page = it->value;
            continue;
        }
        std::string sdim = it->value.substr(0,it->value.find_first_of('=') - 1);
        if(sdim.find('*') || sdim.find('~')) {
            
        } else {
            std::vector<unsigned int> temp = getDimension(sdim);
            MappingPoint mp = {stoi(current_page), temp};
            m_points.insert(mp);
        }   
    }

/*
    for(auto it = m_cl.begin(); it != m_cl.end(); ++it) {
        std::cout << (*it).value << std::endl;
    }
    */

    //transform(str.begin(), str.end(),str.begin(), ::toupper);//대문자로 치환
    
    return true;
}

}