#include "DataProcess.h"
#include "Threading.h"

#include <math.h>

#define MAX_QUOTIENT 5
#define MAX_TIME_DIFFERENCE 2000
#define SUSTAIN_TIME 10000.0f
#define DECAY_TIME 5000.0f

static DataIO::IOManager iom;
DataIO::IOManager* m_IOManager() {
    return &iom;
}

static Logging lgi;
Logging* m_Logging() {
    return &lgi;
}

static TIMESTAMP time_point = Now();

/*
static std::vector<void*> MemoryMap;
static std::vector<std::string> StringMap;
static void PrintText() {

}
*/

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

void initProcess() {
    m_Logging()->createSet("signal",IPCType::SINGLE_FLOAT);
    m_Logging()->createSet("signal1",IPCType::SINGLE_FLOAT);
    m_Logging()->run();
}


struct TimeLine {
    std::vector<unsigned int> id;
    TIMESTAMP timestamp;
};

static TIMESTAMP start_time = Now();


//Thread pool 생성
ThreadPool::ThreadPool pool(5);
std::vector<TimeLine> timeline;

void Log() {
    for(auto it = timeline.begin(); it != timeline.end(); ++it) {
        for(auto iit = it->id.begin(); iit != it->id.end(); ++iit) {
            std::cout << *iit << " ";
        }
        std::cout << ": " << std::chrono::duration_cast<std::chrono::microseconds>(it->timestamp - start_time).count() << std::endl;
    }
}

//데이터 처리
void TLoad(const DataStruct* ds, Signal signal, Neuron *prev, unsigned int i, unsigned int j, unsigned int k) {
    pool.EnqueueJob(ds, signal, prev, i,j,k);
}

const int dir_i[6] = {1,-1,0,0,0,0};
const int dir_j[6] = {0,0,1,-1,0,0};
const int dir_k[6] = {0,0,0,0,1,-1};

void Load(const DataStruct* ds, Signal signal, Neuron *prev, unsigned int i, unsigned int j, unsigned int k) {

    //Validity Check
    if(!ds->data_area) { return; }
    if(!(i < ds->dimSizes[0] && j < ds->dimSizes[1] && k < ds->dimSizes[2])) { return; }
    Neuron* temp = (ds->data_area + i + ds->dimSizes[0] * j + ds->dimSizes[0] * ds->dimSizes[1] * k);
    if((temp->type & 1) == 0) { return; } 
    if((signal.specificity & temp->specificity) == 0) { return; }

    temp->prev = prev;


    TIMESTAMP current_time = std::chrono::steady_clock::now();
    if(signal.timestamp > current_time) { pool.EnqueueJob(ds,signal,prev, i, j, k); return; }
    signal.timestamp = current_time;

    if((temp->type & D8) == D8) {
        for(int offset = 0; offset < 6; ++offset) {
            if(((temp->direction >> offset) & 1) == 1) {
                pool.EnqueueJob(ds, signal, prev, i + dir_i[offset], j + dir_j[offset], k + dir_k[offset]); //prev 전달
            } 
        }
        return;
    }

    //timeline.emplace_back(TimeLine {{i,j,k}, current_time});
    auto time_difference = std::chrono::duration_cast<std::chrono::microseconds>(current_time - temp->timestamp).count();
    if(time_difference > SUSTAIN_TIME) {temp->value *= 1.0f - ((float) (time_difference-SUSTAIN_TIME) / DECAY_TIME); }
    if(time_difference > MAX_TIME_DIFFERENCE) { time_difference = MAX_TIME_DIFFERENCE; }
    if(temp->value < 0) { temp->value = 0.0f; }
    temp->value += signal.value;
    if(temp->value < temp->threshold) { return; }
    int quotient = (int)(temp->value / temp->threshold);
    if(quotient > MAX_QUOTIENT) { quotient = MAX_QUOTIENT; }

    signal.value *= temp->weight;
    if((temp->type & D7) == D7) {
        signal.value = (signal.value > 0) ? -(signal.value) : (signal.value);
    } else {
        signal.value = (signal.value < 0) ? -(signal.value) : (signal.value);
    }
    
    if(i == 0 && j == 0 && k == 0) {
        m_Logging()->addData(0,(FLOAT) std::chrono::duration_cast<std::chrono::milliseconds>(current_time - time_point).count());
        m_Logging()->addData(1,(FLOAT) std::chrono::duration_cast<std::chrono::milliseconds>(current_time - time_point).count());
    }
    else if(i == 2 && j == 0 && k == 0) {
        //m_Logging()->addData(1,(FLOAT) std::chrono::duration_cast<std::chrono::milliseconds>(current_time - time_point).count());
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //printf("%f task[%d][%d][%d]\r\n", m_Monitoring()->getMemoryUsage() ,i,j,k);

    if(m_Monitoring()->getMemoryUsage() > 0.95) { return; }

    for(int offset = 0; offset < 6; ++offset) {
        if(((temp->direction >> offset) & 1) == 1) {
            pool.EnqueueJob(ds, signal, temp, i + dir_i[offset], j + dir_j[offset], k + dir_k[offset]);
        } 
    }

    if(quotient == 1) { return; }
    for(int iter = 1; iter < quotient; ++iter) {
        signal.timestamp = current_time + std::chrono::microseconds((int) ((float) time_difference/quotient)*iter);

        for(int offset = 0; offset < 6; ++offset) {
            if(((temp->direction >> offset) & 1) == 1) {
                pool.EnqueueJob(ds, signal, temp, i + dir_i[offset], j + dir_j[offset], k + dir_k[offset]);
            } 
        }
    }
}