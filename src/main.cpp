#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "job/Counter.h"
#include "job/Job.h"
#include "job/Queue.h"
#include "render/ExampleCube.h"
#include <bx/math.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

constexpr unsigned int windowWidth{1600};
constexpr unsigned int windowHeight{900};

GLFWwindow *setupWindow() {
  glfwInit();
  GLFWwindow *window{glfwCreateWindow(windowWidth, windowHeight,
                                      "bgfx adventures!", nullptr, nullptr)};
  bgfx::PlatformData pd;
  pd.nwh = glfwGetWin32Window(window);
  bgfx::setPlatformData(pd);

  bgfx::Init bgfxInit;
  bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
  bgfxInit.resolution.width = windowWidth;
  bgfxInit.resolution.height = windowHeight;
  bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
  bgfx::init(bgfxInit);

  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f,
                     0);
  bgfx::setViewRect(0, 0, 0, windowWidth, windowHeight);

  return window;
}

std::vector<std::thread> spawnWorkers(job::Counter &jobCounter,
                                      job::Queue &jobs) {
  unsigned int numCores{std::thread::hardware_concurrency()};
  assert(numCores > 1); // make sure we have worker threads
  std::vector<std::thread> workers{};
  while (--numCores) {
    workers.emplace_back([&]() {
      while (true) {
        if (!jobs.empty()) {
          auto job{jobs.pop()};
          job.run();
        }
      }
    });
  }
  return workers;
}

void gameloop() {
  auto [vbh, ibh, program] = render::setup();

  job::Counter jobCounter{};
  job::Queue jobs{jobCounter};
  std::vector<std::thread> workers{spawnWorkers(jobCounter, jobs)};

  unsigned int counter{0};
  while (true) {
    render::RenderCubeParams renderCubeParams{
        &counter, &vbh, &ibh, &program, windowWidth, windowHeight};
    job::Job renderJob{&render::renderCube, &renderCubeParams, jobCounter};
    ++jobCounter;
    renderJob
        .run(); // bgfx API calls must be run on the main thread. See
                // https://github.com/bkaradzic/bgfx/blob/master/src/bgfx.cpp#L36
    while (jobCounter.value() != 0) {
      // busy wait for the moment, let the worker threads consume tasks
      // TODO: explore std::condition_variable
    }
    ++counter;
  }

  render::teardown(vbh, ibh);
}

void teardown(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main() {
  auto window{setupWindow()};
  gameloop();
  teardown(window);
}
