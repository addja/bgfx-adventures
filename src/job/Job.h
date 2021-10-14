#pragma once

#include "Counter.h"

namespace job {

// Encapsulate tasks in objects to be enqueued and shared accross thread workers
// The encapsulation is of functions as the following example

// void foo(void *genericParams) {
//  auto params{reinterpret_cast<unsigned long>(genericParams)};
//  // ...
//}

using EntryPoint = void(void *);

class Job {
private:
  EntryPoint *_entryPoint;
  void *_param;
  Counter &_counter;

public:
  Job(EntryPoint *entryPoint, void *param, Counter &counter)
      : _entryPoint{entryPoint}, _param{param}, _counter(counter) {}
  Job() = delete;
  ~Job();
  Job(Job const &) = default;
  Job(Job &&) = default;

  void run() const;
};

} // namespace job
