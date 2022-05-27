#include "main.h"
#include "DataProcess.h"

#include <thread>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define Point(i,j,k) (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k)

struct shared_data {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int data;
};

int main(){


    int shmid;
    shared_data *d;
    key_t key = 987654;
    void *memory_segment = NULL;

    if ((shmid = shmget(key, sizeof(shared_data), IPC_CREAT | 0666)) < 0)
    {
        printf("shmget failed\n");
        return 0;
    }
    #if REMOVE 
    if (-1 == shmctl(shmid, IPC_RMID, 0))
    {
        printf("공유 메모리 제거 실패\n");
        return -1;
    }
    else
    {
        printf("공유 메모리 제거 성공\n");
    }
    #endif
    
    if ((memory_segment = shmat(shmid, NULL, 0)) == (void *)-1)
    {
        printf("shmat failed\n");
        return 0;
    }

    d = (struct shared_data *)memory_segment;
    //d->lock = PTHREAD_MUTEX_INITIALIZER;
    //d->cond = PTHREAD_COND_INITIALIZER;
    pthread_mutexattr_t attrmutex;
    pthread_mutexattr_init(&attrmutex);
    pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(d->lock),&attrmutex);
    
    pthread_condattr_t attrcond;
    pthread_condattr_init(&attrcond);
    pthread_condattr_setpshared(&attrcond, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&(d->cond), &attrcond);

    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(&(d->lock));
        d->data = i;
        pthread_cond_signal(&(d->cond));
        pthread_mutex_unlock(&(d->lock));
        printf("%d\r\n",i);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    //snprintf((*d).str,sizeof((*d).str), text);


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
