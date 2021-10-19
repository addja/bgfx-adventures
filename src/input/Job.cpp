#include "Job.h"
#include "Manager.h"

namespace input {

void updateInput(void *params) {
  glfwPollEvents();
  auto window{reinterpret_cast<InputJobParams>(params)};

  Manager::instance().futureInput().rotate =
      glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

  Manager::instance().futureInput().escape =
      glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

} // namespace input
