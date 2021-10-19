#include "Queue.h"

namespace job {

void Queue::push(Job const &&job) {
  std::lock_guard<std::mutex> lock(_jobs_mutex);
  _jobs.emplace(job);
  ++_counter;
}

Job Queue::pop() {
  std::lock_guard<std::mutex> lock(_jobs_mutex);
  if (_jobs.empty()) {
    return Job();
  } else {
    auto job{_jobs.front()};
    _jobs.pop();
    return job;
  }
}

bool Queue::empty() const {
  std::lock_guard<std::mutex> lock(_jobs_mutex);
  return _jobs.empty();
}

} // namespace job
