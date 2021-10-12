#include <iostream>

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "render/ExampleCube.h"
#include "render/ShaderLoader.h"

#include <bx/math.h>

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

  unsigned int counter{0};
  while (true) {
    const bx::Vec3 at{0.0f, 0.0f, 0.0f};
    const bx::Vec3 eye{0.0f, 0.0f, -5.0f};
    float view[16];
    bx::mtxLookAt(view, eye, at);
    float proj[16];
    bx::mtxProj(proj, 60.0f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f,
                100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);

    float mtx[16];
    bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
    bgfx::setTransform(mtx);

    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);

    bgfx::submit(0, program);
    bgfx::frame();
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
