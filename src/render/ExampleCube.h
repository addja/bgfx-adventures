#include "bgfx/bgfx.h"
#include "input/Manager.h"
#include "render/ShaderLoader.h"
#include <bx/math.h>
#include <tuple>

namespace render {

// Hardcoded cube used to test the render

struct PosColorVertex {
  float x;
  float y;
  float z;
  uint32_t abgr;
};

static PosColorVertex cubeVertices[]{
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t cubeTriList[]{
    0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6,
    1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};

using RenderCubeParams =
    std::tuple<unsigned int *, bgfx::VertexBufferHandle *,
               bgfx::IndexBufferHandle *, bgfx::ProgramHandle *, unsigned int,
               unsigned int>;

void renderCube(void *params) {
  auto [counter, vbh, ibh, program, windowWidth, windowHeight] =
      *reinterpret_cast<RenderCubeParams *>(params);
  const bx::Vec3 at{0.0f, 0.0f, 0.0f};
  const bx::Vec3 eye{0.0f, 0.0f, -5.0f};
  float view[16];
  bx::mtxLookAt(view, eye, at);
  float proj[16];
  bx::mtxProj(proj, 60.0f,
              static_cast<float>(windowWidth) /
                  static_cast<float>(windowHeight),
              0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
  bgfx::setViewTransform(0, view, proj);

  float mtx[16];
  float rotation{0};
  if (input::Manager::instance().currentInput().rotate) {
    rotation = *counter * 0.01f;
  }
  bx::mtxRotateXY(mtx, rotation, rotation);
  bgfx::setTransform(mtx);

  bgfx::setVertexBuffer(0, *vbh);
  bgfx::setIndexBuffer(*ibh);

  bgfx::submit(0, *program);
  bgfx::frame();
}

decltype(auto) setup() {
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

  bgfx::ShaderHandle vsh{loadShader("vs_cubes.bin")};
  bgfx::ShaderHandle fsh{loadShader("fs_cubes.bin")};
  bgfx::ProgramHandle program{bgfx::createProgram(vsh, fsh, true)};

  return std::make_tuple(vbh, ibh, program);
}

void teardown(bgfx::VertexBufferHandle const &vbh,
              bgfx::IndexBufferHandle const &ibh) {
  bgfx::shutdown();
  bgfx::destroy(ibh);
  bgfx::destroy(vbh);
}

} // namespace render
