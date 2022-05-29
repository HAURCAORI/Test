#include "main.h"
#include "DataProcess.h"

#include <thread>
#include <cstring>
#include "DataIPC.h"

#define Point(i,j,k) (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k)

#define TYPE std::string


int main(){
    

    DataIO::IPCSharedMemory ipc(DataIO::IPC_MODE::SENDER, 987654);
    
    
    for(int i = 0; i < 10; i++) {
        std::vector<FLOAT> vec;
        vec.push_back(i);
        vec.push_back(i+1);
        vec.push_back(i+2);
        std::vector<DataIO::IPCStruct::IPCDataStruct> vecs;
        vecs.push_back(DataIO::IPCStruct::IPCDataStruct("signal",DataIO::IPCStruct::IPCDataType::SINGLE_FLOAT, &vec, vec.size(),sizeof(FLOAT)));
        DataIO::IPCData d = DataIO::IPCStruct::encodeIPCData(vecs);
        ipc.sendData(d);
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    //DataIO::IPCStruct::decodeIPCData(d);
    /*
    std::vector<TYPE> strs;
    for(int i = 0; i < 10; i++) {
        std::string str = "abc" + i;
        strs.push_back(str);
    }
    

    TYPE* d = &strs[0];
    size_t size = strs.size();
    char* vp = (char*) d;
    
    size_t res_size = sizeof(TYPE)*size;
    std::cout << size << "/" << res_size << std::endl;
    char* res = new char(res_size);
    size_t buffer = 10;
    size_t i = 0;
    for(; i < res_size; i += buffer) {
        memcpy(res+i,vp,buffer);
    }
    if(i < res_size) {
        memcpy(res+i,vp,res_size - i);
    }

    TYPE* rd =(TYPE*) vp;
    std::cout << "[result]" << std::endl; 
    for(int i = 0; i < size; i ++) {
        std::cout << *(rd+i) << std::endl;
    }


    delete(res);
    */

   

    /*
    if(!Mapping::Mapping()) { return 0; }
    
    m_IOManager()->loadPage(0);

    //const DataStruct* ds = (m_IOManager()->getDataStruct(0));
    //Neuron* temp = (ds->data_area);//Point(0,0,0);
    //cout << "page : " << temp->value << endl;

    Signal signal = {1.0f, 0xff,std::chrono::steady_clock::now() };
    
    for(int i = 0; i < 5; i++) {
        TLoad(m_IOManager()->getDataStruct(0), signal,nullptr, 0,0,0);
    }
    
    

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Escape" << std::endl;
    Log();
    //m_IOManager()->unloadPage(0);
    
    */
    
    /*
    cout << dio.createPage(0);
     DataIO::loadPage(2);
     DataIO::loadPage(2);
     DataIO::loadPage(3);
     DataIO::loadPage(4);
     DataIO::unloadPage(2);
     DataIO::printPage();
     */
    /*
    Neuron neuron[SectorLayer][SectorSize][SectorSize] = {0};
    
    neuron[0][1][1] = CreateNeuron(0X10);s
    neuron[1][1][1] = CreateNeuron(D5 | 0X03);
    neuron[1][1][2] = CreateNeuron(D5);
    neuron[1][1][0] = CreateNeuron(D5);
    neuron[2][1][2] = CreateNeuron(D2);
    neuron[2][1][0] = CreateNeuron(D1);

    Signal signal = {1.0f, 0xff};
    for(int i = 0; i < 1000; i++) {
        TLoad(&signal, 0,1,1);
        printf("num : %d\r\n", i);
    }
    */
    return 0;
}
