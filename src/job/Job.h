#pragma once

#include "Counter.h"

namespace job {

// Encapsulate tasks in objects to be enqueued and shared accross thread workers

using EntryPoint = void(void *);

class Job {
private:
  EntryPoint *_entryPoint{nullptr};
  void *_param{nullptr};
  Counter &_counter{Counter()};
  bool _valid{false};

public:
  Job(EntryPoint *entryPoint, void *param, Counter &counter)
      : _entryPoint{entryPoint}, _param{param},
        _counter(counter), _valid{true} {}
  Job() {}

  void run() const;
  bool valid() const { return _valid; }
};

} // namespace job
