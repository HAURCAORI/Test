#include "main.h"
#include "DataProcess.h"
#include "DataIO.h"


int main(){
    
    IOManager a;
    a.deletePage(0);
    a.createPage(0);
    a.loadPage(0);
    int* t = a.getPageFile(0).data_area;
    cout << "page : " << *(t) << endl;
    int* p = static_cast<int*>(a.getPointer(0));
    for(int i = 0; i < 10; i++) {
        std::cout << *(p+i) << std::endl;
    }
    a.unloadPage(0);
    
    
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