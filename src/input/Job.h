#pragma once

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace input {

using InputJobParams = GLFWwindow *;

void updateInput(void *);

} // namespace input
