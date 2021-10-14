#pragma once

#include "Counter.h"
#include "Job.h"
#include <mutex>
#include <queue>

namespace job {

class Queue {
public:
  Queue(Counter &counter) : _counter(counter) {}
  void push(Job const &&job);
  [[nodiscard]] Job pop();
  [[nodiscard]] bool empty() const;

private:
  mutable std::mutex _jobs_mutex;
  std::queue<Job> _jobs;
  Counter &_counter;
};

} // namespace job
