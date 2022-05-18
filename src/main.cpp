#include "main.h"
#include "DataProcess.h"

#include <thread>

#define Point(i,j,k) (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k)

int main(){
    if(!Mapping::Mapping()) { return 0; }
    
    m_IOManager()->loadPage(0);

    //const DataStruct* ds = (m_IOManager()->getDataStruct(0));
    //Neuron* temp = (ds->data_area);//Point(0,0,0);
    //cout << "page : " << temp->value << endl;
   
    Signal signal = {1.0f, 0xff};
    for(int i = 0; i < 1; i++) {
        TLoad(m_IOManager()->getDataStruct(0), &signal, 0,0,0);
        printf("num : %d\r\n", i);
    }
    

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //m_IOManager()->unloadPage(0);
    
    
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