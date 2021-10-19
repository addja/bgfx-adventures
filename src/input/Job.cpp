#include "Job.h"
#include "Manager.h"

namespace input {

void updateInput(void *) { Manager().futureInput().foo = true; }

} // namespace input
