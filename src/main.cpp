#include <stdio.h>
#include <stdlib.h>
#define SHAPE_SIZE 4
#define CARD_MAX_VALUE 13
struct card {
    const char* shape;
    int number;
};
const char* shape[SHAPE_SIZE] = {"Hearts", "Diamonds","Clubs", "Spades"};

int main() {
    card card_list[52];
    //initialize card;
    for(int i = 0; i < SHAPE_SIZE; i++) {
        for(int j = 0; j < CARD_MAX_VALUE; j++) {
            (card_list + i*CARD_MAX_VALUE + j)->shape = shape[i];
            (card_list + i*CARD_MAX_VALUE + j)->number = (j+1);
        }
    }
    
    //shuffle
    for(int i = 0; i < SHAPE_SIZE*CARD_MAX_VALUE; i++) {
        int index = rand() % (SHAPE_SIZE*CARD_MAX_VALUE);
        card temp = *(card_list+i);
        *(card_list+i) = *(card_list+index);
        *(card_list+index) = temp;
    }

    //print
    for(int i = 0; i < SHAPE_SIZE*CARD_MAX_VALUE; i++) {
        printf("Card %2d: %10s, %2d\r\n", i+1,card_list[i].shape, card_list[i].number);
    }
    return 0;

}


/*#include "main.h"
#include "DataProcess.h"

#include <thread>

#define Point(i,j,k) (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k)

int main(){
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
    
    return 0;
}
*/