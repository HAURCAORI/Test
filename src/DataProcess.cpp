#include "DataProcess.h"
#include "Threading.h"

#include <math.h>

static DataIO::IOManager iom;

DataIO::IOManager* m_IOManager() {
    return &iom;
}

template <class... Args>
void ThreadPool::ThreadPool::EnqueueJob(Args&&... args) {
  auto job = [args...](){ Load(args...); };
  {
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.emplace(std::move(job));
  }
  cv_job_q_.notify_one();
}

ThreadPool::ThreadPool pool(5);

void TLoad(const DataStruct* ds, Signal *signal, unsigned int i, unsigned int j, unsigned int k) {
    pool.EnqueueJob(ds, signal, i,j,k);
}

void Load(const DataStruct* ds,Signal *signal, unsigned int i, unsigned int j, unsigned int k) {
    if(i > 40) { i = 0; j++; }
    if(j > 40) { j = 0; k++; }
    if(!ds->data_area) { return; }
    if(!(i < ds->dimSizes[0] && j < ds->dimSizes[1] && k < ds->dimSizes[2])) { return; }
    Neuron* temp = (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k);
    
    //std::this_thread::sleep_for(std::chrono::nanoseconds(3));
    printf("%f task : %d\r\n", m_Monitoring()->getMemoryUsage() , temp->a);
    

    if(m_Monitoring()->getMemoryUsage() > 0.95) { return; }
    pool.EnqueueJob(ds, signal, i+1,j,k);

}
/*
bool Load(Neuron (*target)[SectorSize][SectorSize], int i, int j, int k, Signal *signal)
{
    
    BEGIN_CHRONO
    if (i >= SectorLayer || j >= SectorSize || k >= SectorSize)
    {
        return false;
    }
    if (i < 0 || j < 0 || k < 0)
    {
        return false;
    }
    //printf("Load[%i,%i,%i]", i, j, k);

    if (((target[i][j][k].direction) & 1) == 1)
    {
        TLoad(target, i, j, k + 1, signal);
    }
    if (((target[i][j][k].direction >> 1) & 1) == 1)
    {
        TLoad(target, i, j, k - 1, signal);
    }
    if (((target[i][j][k].direction >> 2) & 1) == 1)
    {
        TLoad(target, i, j + 1, k, signal);
    }
    if (((target[i][j][k].direction >> 3) & 1) == 1)
    {
        TLoad(target, i, j - 1, k, signal);
    }
    if (((target[i][j][k].direction >> 4) & 1) == 1)
    {
        TLoad(target, i + 1, j, k, signal);
    }
    if (((target[i][j][k].direction >> 5) & 1) == 1)
    {
        TLoad(target, i - 1, j, k, signal);
    }
    END_CHRONO
    
    return true;
}
*/