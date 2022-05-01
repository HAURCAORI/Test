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
#include "Monitoring.h"

Monitoring::Monitoring monitoring;

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
    if(monitoring.getCpuUsage() > 70) {
      printf("pause");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if(monitoring.getMemoryUsage() > 0.95) {
      printf("RAM OVER\r\n");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      //worker_threads_.emplace_back([this]() { this->WorkerThread(); });
      //return;
    }
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
  auto job = [args...](){ Load(args...); };
  {
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.emplace(std::move(job));
  }
  cv_job_q_.notify_one();
}

}  // namespace ThreadPool

ThreadPool::ThreadPool pool(5);

void TLoad(int i, int j, int k, Signal *signal) {
    pool.EnqueueJob(i, j, k, signal);
}
static double sum = 0;
static double mean = 0;
static std::atomic<int> count(0);
void Load(int i, int j, int k, Signal *signal) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(3));
  ++count;
  sum += monitoring.getMemoryUsage();
  if(count >= 100000) {
    mean = sum / (double) count;
    printf("%f\r\n", mean);
    count = 0;
    sum = 0;
  }
  //printf("%f task : %d\r\n", ram, i);
  if(i > 20) { return; }
  if(monitoring.getMemoryUsage() > 0.95) { return; }
  TLoad(i+1,1,1, signal);
}