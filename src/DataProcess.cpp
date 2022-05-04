#include "DataProcess.h"
#include "Threading.h"
#include <math.h>
#include <chrono>
#define BEGIN_CHRONO std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#define END_CHRONO std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;

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

void TLoad() {
    //pool.EnqueueJob();
}

void Load(int i, int j, int k, Signal *signal) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(3));
    printf("%f task : %d\r\n", monitoring.getMemoryUsage() , i);
    if(i > 20) { return; }
    if(monitoring.getMemoryUsage() > 0.95) { return; }
    pool.EnqueueJob(i,j,k, signal);
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