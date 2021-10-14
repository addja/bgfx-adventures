#pragma once

#include <atomic>

namespace job {

class Job;
class Queue;

class Counter {
  friend Job;
  friend Queue;

private:
  std::atomic_uint _value{0};
  void operator++() { ++_value; }
  void operator--() { --_value; }

public:
  [[nodiscard]] unsigned int value() const { return _value; }
};

} // namespace job
