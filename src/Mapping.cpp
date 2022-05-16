#include "Mapping.h"
#include "Calculate.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <exception>

#define cmap(arg) {#arg, Command::arg}
#define firstSplitString(x) x.substr(1, x.find_first_of(' ') - 1)
#define secondSplitString(x) x.substr(x.find_first_of(' ') + 1)

#define TIME_ESTIMATE
struct MappingPoint
{
    PAGE id;
    std::vector<unsigned int> point;

    bool operator==(const MappingPoint &mp) const
    {
        if (this->id == mp.id)
            if (this->point == mp.point)
                return true;
        return false;
    }

    struct HashFunction
    {
        size_t operator()(const MappingPoint &mp) const
        {
            std::hash<unsigned int> hasher;
            size_t seed = mp.id;
            for (unsigned int i : mp.point)
            {
                seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
};

namespace Mapping
{

enum class ErrorType
{
    GENERAL,
    PREPROCESSING,
    PROCESSING,
    SYNTAX
};

inline void errorMsg(std::string &&e, int line = 0, ErrorType type = ErrorType::GENERAL)
{
    switch (type)
    {
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

inline void warningMsg(std::string &&e, int line = 0, ErrorType type = ErrorType::GENERAL)
{
    switch (type)
    {
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

Neuron createNeuron()
{
    Neuron temp = {0x00, 0xff, 0x00, 0.0f, 0.0f, random_threshold(), random_weight()};
    return temp;
}
Neuron createNeuron(DIRECTION direction)
{
    Neuron temp = {0x00, 0xff, direction, 0.0f, 0.0f, random_threshold(), random_weight()};
    return temp;
}
Neuron createNeuron(DIRECTION direction, float threshold)
{
    Neuron temp = {0x00, 0xff, direction, 0.0f, 0.0f, threshold, random_weight()};
    return temp;
}
Neuron createNeuron(DIRECTION direction, float threshold, float weight)
{
    Neuron temp = {0x00, 0xff, direction, 0.0f, 0.0f, threshold, weight};
    return temp;
}

void writeDataStruct(FILE *stream, long pos, Neuron &&sender)
{
    fseek(stream, pos, SEEK_SET);
    fwrite(&sender, sizeof(Neuron), 1, stream);
}
void writeDataStruct(FILE *stream, const std::vector<unsigned int>& point, const std::vector<unsigned int>& dimsizes, Neuron &sender)
{
    long pos = 0;
    for(size_t i = 0; i < point.size(); i++) {
        long temp = point[i];
        for(size_t j = 0; j < i; j++) {
            temp *= dimsizes[j];
        }
        pos+=temp;
    }
    pos += (dimsizes.size() + 1)*4; //header
    fseek(stream, pos, SEEK_SET);
    fwrite(&sender, sizeof(Neuron), 1, stream);
}

// { 1, 2, 3 } 형식의 데이터를 벡터로
std::vector<unsigned int> getDimension(std::string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());

    std::vector<unsigned int> vec;
    if ((str.front() == '{' && str.back() == '}') || (str.front() == '[' && str.back() == ']'))
    {
        str = str.substr(1, str.size() - 2);
        std::vector<std::string> temp = split(str, ",");
        vec.reserve(temp.size());
        try
        {
            for (auto it = temp.begin(); it != temp.end(); ++it)
            {
                vec.push_back(stoi(*it));
            }
        }
        catch (std::exception e)
        {
            errorMsg("'{" + str + "}' Can't Convert to Vector.");
        }
    }
    else
    {
        errorMsg("'" + str + "' Expression Should Include '{}' or '[]'.");
    }
    return vec;
}

// '1,2~3,*'
// 초기 조건 vecs 는 empty vector, strs와 dimsizes는 모두 n개의 원소 가짐
std::vector<std::vector<unsigned int>> getDimensions(std::vector<std::string> &strs, const std::vector<unsigned int> &dimsizes, std::vector<unsigned int> vec)
{
    std::vector<std::vector<unsigned int>> ret;
    if (strs.empty())
    {
        return ret;
    }
    std::string str = strs.front();
    strs.erase(strs.begin());

    int index = vec.size();
    size_t iter = index;
    for (unsigned int i = index; i < dimsizes.size(); i++)
    {
        if (isInteger(str))
        {
            unsigned int value = stoi(str);
            if (value >= dimsizes[i])
            {
                break;
            }
            vec.push_back(value);
            if (!strs.empty())
            {
                str = strs.front();
                strs.erase(strs.begin());
            }
            ++iter;
        }
        else if (str == "*")
        {
            for (unsigned int j = 0; j < dimsizes[i]; j++)
            {
                std::vector<std::string> temp = strs;
                temp.insert(temp.begin(), std::to_string(j));
                for (auto v : getDimensions(temp, dimsizes, vec))
                {
                    ret.push_back(v);
                }
            }
            return ret;
        }
        else
        {
            std::vector<std::string> list;
            if (str.find('|') != std::string::npos)
            {
                std::vector<std::string> temp = split(str, "|");
                for (auto it = temp.begin(); it != temp.end(); ++it)
                {
                    list.push_back(*it);
                }
            }
            else if (str.find('~') != std::string::npos)
            {
                std::vector<std::string> temp = split(str, "~");
                if (temp.size() == 2 && isInteger(temp[0]) && isInteger(temp[1]))
                {
                    for (int i = stoi(temp[0]); i <= stoi(temp[1]); i++)
                    {
                        list.push_back(std::to_string(i));
                    }
                }
                else
                {
                    warningMsg("'" + str + "' Can't Convert to Vector.");
                }
            }
            else
            {
                warningMsg("'" + str + "' Can't Convert to Vector.");
                return ret;
            }

            for (auto l : list)
            {
                std::vector<std::string> temp = strs;
                temp.insert(temp.begin(), l);
                for (auto v : getDimensions(temp, dimsizes, vec))
                {
                    ret.push_back(v);
                }
            }
            return ret;
        }
    }
    if (iter == dimsizes.size())
    {
        ret.push_back(vec);
    }
    return ret;
}

std::vector<std::vector<unsigned int>> getDimensions(std::string &str, const std::vector<unsigned int> &dimsizes)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    if ((str.front() == '{' && str.back() == '}') || (str.front() == '[' && str.back() == ']'))
    {
        str = str.substr(1, str.size() - 2);
    }
    else
    {
        warningMsg("'" + str + "' Expression Should Include '{}' or '[]'.");
    }

    std::vector<std::string> strs = split(str, ",");
    if (strs.size() != dimsizes.size())
    {
        std::vector<std::vector<unsigned int>> ret;
        warningMsg("'{" + str + "}' Dimensions dismatch. (" + std::to_string(strs.size()) + " <> " + std::to_string(dimsizes.size()) + ")");
        return ret;
    }
    std::vector<unsigned int> temp;
    return getDimensions(strs, dimsizes, temp);
}

// 1. 모든 코드 라인을 읽고 정형화
// 2. 정형화된 코드 중 preprocessing 진행
// 3. 정형화된 코드 중 dataprocessing을  링킹을 위해 목록화
// 4. data 처리(linking, write)

enum class CommandType
{
    NONE,
    PREPROCESSING,
    DATAPROCESSING
};

enum class Command
{
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

static std::unordered_map<std::string, Command> const command_table = {
    cmap(DIMENSION), cmap(DEFAULT), cmap(DEFINE), cmap(FUNCTION), {"PAGE", Command::PAGE}, cmap(DATA)};
Command matchingCommand(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    auto it = command_table.find(str);
    if (it != command_table.end())
    {
        return it->second;
    }
    else
    {
        return Command::NONE;
    }
}

enum class Attribute
{
    NONE,
    TYPE,
    SPECIFICITY,
    DIRECTION,
    THRESHOLD,
    WEIGHT,
    TARGET,
    OUT
};

struct AttributeData
{
    Attribute attribute;
    std::vector<std::string> args;
};

struct codeLine
{
    CommandType type;
    Command command;
    int group;
    int line;
    std::string value;
};

bool Mapping()
{

    std::cout << "===== Mapping =====" << std::endl;
    std::string address = (std::string)Path + "mapping";
    std::ifstream file(address);

    std::vector<codeLine> m_cl;

    bool error = false;

    // 1. 코드 정형화
    {
        #ifdef TIME_ESTIMATE
        BEGIN_CHRONO
        #endif
        if (file.is_open())
        {
            std::string str; //get text line
            char token;      //first text
            int group = 0;   //group
            int line = 0;
            while (!file.eof())
            {
                std::getline(file, str);
                ++line;
                if (str.empty())
                {
                    continue;
                }
                codeLine tcl;
                tcl.line = line;
                token = str.front();
                switch (token)
                {
                case '#':
                {
                    tcl.type = CommandType::PREPROCESSING;
                    tcl.group = 0;
                    tcl.value = secondSplitString(str);
                    tcl.command = matchingCommand(firstSplitString(str));
                    m_cl.push_back(tcl);
                }
                break;

                case '$':
                {
                    group++;
                    tcl.type = CommandType::DATAPROCESSING;
                    tcl.group = group;
                    tcl.value = secondSplitString(str);
                    Command tc = matchingCommand(firstSplitString(str));
                    if (tc == Command::PAGE)
                    {
                        tcl.command = tc;
                    }
                    else
                    {
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
                    if (str.at(1) == '/')
                    {
                        continue;
                    }
                    break;

                default:
                    warningMsg("Can't encode line.", line, ErrorType::SYNTAX);
                    continue;
                    break;
                }
            }
        }
        else
        {
            errorMsg("Can't open 'mapping' file.");
        }
        file.close();

        if (error)
        {
            errorMsg("Regularizing Fail.");
            return false;
        }
        error = false;
        #ifdef TIME_ESTIMATE
        std::cout << "[Time Estimate] 1. regulate";
        END_CHRONO
        #endif
    }

    // 2. preprocessing
    bool valid = true;
    unsigned int m_dimension = 0;
    std::vector<unsigned int> m_dimsizes;
    m_dimsizes.reserve(5);
    {
        #ifdef TIME_ESTIMATE
        BEGIN_CHRONO
        #endif
        for (auto it = m_cl.begin(); it != m_cl.end();)
        {
            if (it->type != CommandType::PREPROCESSING)
            {
                ++it;
                continue;
            }
            switch (it->command)
            {
            case Command::DIMENSION:
            {
                m_dimsizes = getDimension(it->value);
                if (m_dimsizes.empty())
                {
                    error = true;
                    errorMsg("Dimension is Empty.", it->line, ErrorType::PREPROCESSING);
                    break;
                }
                m_dimension = m_dimsizes.size();
            }
            break;

            case Command::DEFAULT:
            {
            }
            break;

            case Command::DEFINE:
            {
            }
            break;

            case Command::FUNCTION:
            {
            }
            break;

            default:
                warningMsg("Undefined Command.", it->line, ErrorType::PREPROCESSING);
                break;
            }
            it = m_cl.erase(it);
        }

        // 유효성 판단
        for (auto it = m_dimsizes.begin(); it != m_dimsizes.end(); ++it)
        {
            if (*it == 0)
            {
                valid = false;
                errorMsg("Dimension is 0.");
                break;
            }
            if (*it > MAX_SECTOR)
            {
                valid = false;
                errorMsg("Dimension Out of Range.");
                break;
            }
        }

        if (m_dimension == 0)
        {
            error = true;
            errorMsg("Must define '#DIMENSION'.");
        }

        if (error || !valid)
        {
            errorMsg("Preprocessing Fail.");
            return false;
        }
        error = false;

        #ifdef TIME_ESTIMATE
        std::cout << "[Time Estimate] 2. preprocessing";
        END_CHRONO
        #endif
    }

    // 3. linking
    std::multimap<std::string,MappingPoint> m_points;
    //std::unordered_map<MappingPoint, std::string, MappingPoint::HashFunction> m_points;
    //std::unordered_set<MappingPoint, MappingPoint::HashFunction> m_points;
    {
        #ifdef TIME_ESTIMATE
        BEGIN_CHRONO
        #endif
        
        std::string current_page = "0";
        for (auto it = m_cl.begin(); it != m_cl.end();)
        {
            if (it->type != CommandType::DATAPROCESSING)
            {
                ++it;
                continue;
            }
            if (it->command == Command::PAGE)
            {
                if (!isInteger(it->value))
                {
                    error = true;
                    errorMsg("PAGE shold be Integer.", it->line, ErrorType::PROCESSING);
                    break;
                }
                current_page = it->value;
                ++it;
                continue;
            }
            if (it->value.find("=>") == std::string::npos && it->value.find("==") == std::string::npos)
            {
                warningMsg("'" + it->value + "' Not Defined Data.", it->line, ErrorType::PROCESSING);
                it = m_cl.erase(it);
                continue;
            }
            std::string sdim = it->value.substr(0, it->value.find_first_of('=') - 1);

            for (auto t : getDimensions(sdim, m_dimsizes))
            {
                //std::cout<< "point" << current_page << " / " << it->line << std::endl; printVector(t);

                MappingPoint tmp({stoi(current_page), t});
                m_points.insert(std::make_pair(it->value,tmp));
            }
            ++it;
        }
        if (error)
        {
            errorMsg("Linking Fail.");
            return false;
        }
        error = false;
        #ifdef TIME_ESTIMATE
        std::cout << "[Time Estimate] 3. linking";
        END_CHRONO
        #endif
    }
    // 4. dataprocessing

    {
        #ifdef TIME_ESTIMATE
        BEGIN_CHRONO
        #endif
        std::string current_page;
        FILE* stream = nullptr;
        std::string address;
        for (auto it = m_cl.begin(); it != m_cl.end(); ++it)
        {
            if (it->command == Command::PAGE)
            {
                current_page = it->value;
                if(stream != nullptr) {
                    fclose(stream);
                }
                address = (std::string)Path + current_page;
                stream = fopen(address.c_str(), "wb");
                if(!stream) { 
                    errorMsg("'" + address + "' Fail to Open File." , 0,ErrorType::PROCESSING);
                    error = true;
                    break;
                }
                //파일 초기화
                const int header = (m_dimsizes.size() + 1)*4;
                unsigned int data_size = 1;
                fwrite(&header, sizeof(int),1,stream);
                for(unsigned int x : m_dimsizes) {
                    fwrite(&x, sizeof(unsigned int), 1, stream);
                    data_size *= x;
                }
                Neuron temp = Mapping::createNeuron();
                fwrite(&temp, sizeof(Neuron), data_size, stream);

                continue;
            }
            if(!stream) {
                errorMsg("Data Must Included in the Page." , it->line,ErrorType::PROCESSING);
                error = true;
                break;
            }
            for (auto p_it = m_points.lower_bound(it->value); p_it != m_points.upper_bound(it->value); ++p_it)
	        {
                std::vector<unsigned int> tp = p_it->second.point;
                
                Neuron tn = createNeuron();
                writeDataStruct(stream,tp,m_dimsizes, tn);
		        //std::cout << "[" << p_it->first << "]" << std::endl;
	        }
        }

        if(stream) { fclose(stream); }

        if (error)
        {
            errorMsg("Processing Fail.");
            return false;
        }
        #ifdef TIME_ESTIMATE
        std::cout << "[Time Estimate] 4. dataprocessing";
        END_CHRONO
        #endif
    }
    /*
    for(auto it = m_cl.begin(); it != m_cl.end(); ++it) {
        std::cout << (*it).value << std::endl;
    }
    */

    //transform(str.begin(), str.end(),str.begin(), ::toupper);//대문자로 치환

    return true;
}

} // namespace Mapping