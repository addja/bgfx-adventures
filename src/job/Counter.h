#pragma once

#include <atomic>

namespace job {

class Job;

class Counter {
  friend Job;

private:
  std::atomic_uint _value{0};
  void operator--() { --_value; }

public:
  [[nodiscard]] unsigned int value() const { return _value; }
  void operator++() { ++_value; }
};

} // namespace job
