#pragma once

#include "Input.h"
#include <memory>
#include <utility>

namespace input {

// Singleton holding the input state.
// Implemented with a double buffer design pattern.

class Manager {
public:
  static Manager &instance() { return _instance; }
  Input const &currentInput() const { return _current; }
  Input &futureInput() { return _future; }
  void step() { std::swap(_future, _current); };

private:
  static Manager _instance;
  Input _current;
  Input _future;
};

} // namespace input
