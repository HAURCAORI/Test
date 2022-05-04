#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "Monitoring.h"

static Monitoring::Monitoring monitoring;

namespace ThreadPool {
class ThreadPool {
 public:
  ThreadPool(size_t num_threads);
  ~ThreadPool();

  template <class... Args>
  void EnqueueJob(Args&&... args);

 private:
  size_t num_threads_;
  std::vector<std::thread> worker_threads_;
  std::queue<std::function<void()>> jobs_;
  std::condition_variable cv_job_q_;
  std::mutex m_job_q_;

  bool stop_all;

  void WorkerThread();
};
}