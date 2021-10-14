#include "Job.h"

namespace job {

Job::~Job() { --_counter; }

void Job::run() const { _entryPoint(_param); }

} // namespace job
