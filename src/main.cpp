#include "main.h"
#include "DataProcess.h"

int main(){
    
    m_IOManager()->deletePage(0);
    m_IOManager()->createPage(0);
    m_IOManager()->loadPage(0);
    //Neuron* t = a.getPageFile(0).ds.data_area;
    //cout << "page : " << *(t) << endl;
    //int* p = static_cast<int*>(a.getPointer(0));
    /*
    for(int i = 0; i < 10; i++) {
        //std::cout << *(p+i) << std::endl;
        std::cout << (*(t+i)).b << std::endl;
    }
    */
   /*
    Signal signal = {1.0f, 0xff};
    for(int i = 0; i < 1; i++) {
        TLoad(iom.getDataStruct(0), &signal, 0,1,1);
        printf("num : %d\r\n", i);
    }
    */

    //iom.unloadPage(0);
    
    
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
    
    neuron[0][1][1] = CreateNeuron(0X10);
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
    //std::this_thread::sleep_for(std::chrono::minutes(5));
    return 0;
}