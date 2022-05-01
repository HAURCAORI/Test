#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "DataProcess.h"


namespace ThreadPool {
class ThreadPool {
 public:
  ThreadPool(size_t num_threads);
  ~ThreadPool();

  // job 을 추가한다.
  template <class... Args>
  void EnqueueJob(Args&&... args);

 private:
  // 총 Worker 쓰레드의 개수.
  size_t num_threads_;
  // Worker 쓰레드를 보관하는 벡터.
  std::vector<std::thread> worker_threads_;
  // 할일들을 보관하는 job 큐.
  std::queue<std::function<void()>> jobs_;
  // 위의 job 큐를 위한 cv 와 m.
  std::condition_variable cv_job_q_;
  std::mutex m_job_q_;

  // 모든 쓰레드 종료
  bool stop_all;

  // Worker 쓰레드
  void WorkerThread();
};

ThreadPool::ThreadPool(size_t num_threads): num_threads_(num_threads), stop_all(false) {
  worker_threads_.reserve(num_threads_);
  for (size_t i = 0; i < num_threads_; ++i) {
    worker_threads_.emplace_back([this]() { this->WorkerThread(); });
  }
}

void ThreadPool::WorkerThread() {
  while (true) {
    std::unique_lock<std::mutex> lock(m_job_q_);
    cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
    if (stop_all && this->jobs_.empty()) {
      return;
    }

    // 맨 앞의 job 을 뺀다.
    std::function<void()> job = std::move(jobs_.front());
    jobs_.pop();
    lock.unlock();

    // 해당 job 을 수행한다 :)
    job();
  }
}

ThreadPool::~ThreadPool() {
  std::cout << "소멸자 호출\r\n";
  stop_all = true;
  cv_job_q_.notify_all();

  for (auto& t : worker_threads_) {
    t.join();
  }
}

template <class... Args>
void ThreadPool::EnqueueJob(Args&&... args) {
  if (stop_all) {
    //throw std::runtime_error("ThreadPool 사용 중지됨");
  }
  auto job = [args...]{ Load(args...); };
  {
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.push(job);
  }
  cv_job_q_.notify_one();
}

}  // namespace ThreadPool

ThreadPool::ThreadPool pool(3);

void TLoad(Neuron (*target)[SectorSize][SectorSize], int i, int j, int k, Signal *signal) {
    //std::future<bool> future;
    //future = pool.EnqueueJob(Load, target, i, j, k, signal);
    //future.wait();
    pool.EnqueueJob(target, i, j, k, signal);
}

void Load(Neuron (*target)[SectorSize][SectorSize], int i, int j, int k, Signal *signal) {
  
  printf("task : %d\r\n", i);
  if(i > 100) { return; }
  TLoad(target,i+1,1,1, signal);
}