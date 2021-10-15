#pragma once

#include "Counter.h"

namespace job {

// Encapsulate tasks in objects to be enqueued and shared accross thread workers

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

  void run() const;
};

} // namespace job
