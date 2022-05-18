#include "DataProcess.h"
#include "Threading.h"

#include <math.h>

static DataIO::IOManager iom;
DataIO::IOManager* m_IOManager() {
    return &iom;
}

//Thread 목적 함수 연결
template <class... Args>
void ThreadPool::ThreadPool::EnqueueJob(Args&&... args) {
  auto job = [args...](){ Load(args...); };
  {
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.emplace(std::move(job));
  }
  cv_job_q_.notify_one();
}

//Thread pool 생성
ThreadPool::ThreadPool pool(5);

//데이터 처리
void TLoad(const DataStruct* ds, Signal *signal, unsigned int i, unsigned int j, unsigned int k) {
    pool.EnqueueJob(ds, signal, i,j,k);
}

const int dir_i[6] = {1,-1,0,0,0,0};
const int dir_j[6] = {0,0,1,-1,0,0};
const int dir_k[6] = {0,0,0,0,1,-1};

void Load(const DataStruct* ds,Signal *signal, unsigned int i, unsigned int j, unsigned int k) {
    if(!ds->data_area) { return; }
    if(!(i < ds->dimSizes[0] && j < ds->dimSizes[1] && k < ds->dimSizes[2])) { return; }
    Neuron* temp = (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k);
    

    //std::this_thread::sleep_for(std::chrono::nanoseconds(3));
    printf("%f task[%d][%d][%d]\r\n", m_Monitoring()->getMemoryUsage() ,i,j,k);
    
    if(m_Monitoring()->getMemoryUsage() > 0.95) { return; }
    for(int offset = 0; offset < 6; offset++) {
        if(((temp->direction >> offset) & 1) == 1) {
            pool.EnqueueJob(ds, signal, i + dir_i[offset], j + dir_j[offset], k + dir_k[offset]);
        }
    }
}