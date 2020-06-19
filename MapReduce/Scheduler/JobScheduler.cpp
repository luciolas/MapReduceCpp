#include "JobScheduler.h"


void Scheduler::thread_main_loop()
{
  I_FnPackage * fn = nullptr;
  while (true) 
  {
    {
      std::unique_lock<std::mutex> lock(mu_);
      work_signal.wait(lock, [this]() { return !queue_.empty() || isStopped; });
      if (isStopped) 
      {
        return;
      }
      fn = queue_.front();
      queue_.pop();
    }
    {
      fn->invoke();
    }
  }
}

Scheduler::Scheduler(int workers) :isStopped{ false }, workers_{}, work_signal{}
{
  if (workers < 2) {
    // throw
    workers = 2;
  }
  workers -= 1;
  for (int i = 0; i < workers; i++) {
    workers_.push(i);
    worker_threads_.emplace_back(std::thread(&Scheduler::thread_main_loop, this));
  }
}

void Scheduler::Start()
{

}

bool Scheduler::Schedule(I_FnPackage* fn) {
  {
    std::unique_lock<std::mutex> lock(mu_);
    queue_.push(fn); 
  }
  work_signal.notify_one();
  return true;
}

