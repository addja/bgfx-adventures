#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "job/Counter.h"
#include "job/Job.h"
#include "job/Queue.h"
#include "render/ExampleCube.h"
#include "render/ShaderLoader.h"
#include <bx/math.h>
#include <iostream>
#include <tuple>
#include <utility>

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

GLFWwindow *setupWindow() {
  glfwInit();
  GLFWwindow *window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                      "bgfx adventures!", nullptr, nullptr)};
  bgfx::PlatformData pd;
  pd.nwh = glfwGetWin32Window(window);
  bgfx::setPlatformData(pd);

  bgfx::Init bgfxInit;
  bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
  bgfxInit.resolution.width = WINDOW_WIDTH;
  bgfxInit.resolution.height = WINDOW_HEIGHT;
  bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
  bgfx::init(bgfxInit);

  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f,
                     0);
  bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  return window;
}

using RenderCubeParams =
    std::tuple<unsigned int *, bgfx::VertexBufferHandle *,
               bgfx::IndexBufferHandle *, bgfx::ProgramHandle *>;

void renderCube(void *params) {
  auto [counter, vbh, ibh, program] =
      *reinterpret_cast<RenderCubeParams *>(params);
  const bx::Vec3 at{0.0f, 0.0f, 0.0f};
  const bx::Vec3 eye{0.0f, 0.0f, -5.0f};
  float view[16];
  bx::mtxLookAt(view, eye, at);
  float proj[16];
  bx::mtxProj(proj, 60.0f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f,
              100.0f, bgfx::getCaps()->homogeneousDepth);
  bgfx::setViewTransform(0, view, proj);

  float mtx[16];
  bx::mtxRotateXY(mtx, *counter * 0.01f, *counter * 0.01f);
  bgfx::setTransform(mtx);

  bgfx::setVertexBuffer(0, *vbh);
  bgfx::setIndexBuffer(*ibh);

  bgfx::submit(0, *program);
  bgfx::frame();
}

void gameloop() {
  bgfx::VertexLayout pcvDecl;
  pcvDecl.begin()
      .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
      .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
      .end();
  bgfx::VertexBufferHandle vbh{bgfx::createVertexBuffer(
      bgfx::makeRef(render::cubeVertices, sizeof(render::cubeVertices)),
      pcvDecl)};
  bgfx::IndexBufferHandle ibh{bgfx::createIndexBuffer(
      bgfx::makeRef(render::cubeTriList, sizeof(render::cubeTriList)))};

  bgfx::ShaderHandle vsh{render::loadShader("vs_cubes.bin")};
  bgfx::ShaderHandle fsh{render::loadShader("fs_cubes.bin")};
  bgfx::ProgramHandle program{bgfx::createProgram(vsh, fsh, true)};

  job::Counter jobCounter{};
  job::Queue jobs{jobCounter};

  unsigned int counter{0};

  RenderCubeParams renderCubeParams{&counter, &vbh, &ibh, &program};
  while (true) {
    job::Job renderJob{&renderCube, &renderCubeParams, jobCounter};
    jobs.push(std::move(renderJob));
    while (jobCounter.value() != 0) {
      // busy wait for the moment, let the worker threads consume tasks
      // TODO: explore std::condition_variable
    }
    ++counter;
  }

  bgfx::shutdown();
  bgfx::destroy(ibh);
  bgfx::destroy(vbh);
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
