#include "Job.h"

namespace job {

void Job::run() const {
  _entryPoint(_param);
  --_counter;
}

} // namespace job
