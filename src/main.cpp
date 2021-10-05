#include <iostream>

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include <bx/math.h>

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

struct PosColorVertex {
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] {
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] {
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

bgfx::ShaderHandle loadShader(std::string const & fileName) {
    std::string shaderPath{ []() {
		switch (bgfx::getRendererType()) {
			case bgfx::RendererType::Noop:
			case bgfx::RendererType::Direct3D9:  return "shaders/dx9/";
			case bgfx::RendererType::Direct3D11:
			case bgfx::RendererType::Direct3D12: return "shaders/dx11/";
			case bgfx::RendererType::Gnm:        return "shaders/pssl/";
			case bgfx::RendererType::Metal:      return "shaders/metal/";
			case bgfx::RendererType::OpenGL:     return "shaders/glsl/";
			case bgfx::RendererType::OpenGLES:   return "shaders/essl/";
			case bgfx::RendererType::Vulkan:     return "shaders/spirv/";
		}
    }() };
    shaderPath = "../bin/Debug/" + shaderPath; // hack to make it work with current project layout
    std::string filePath{ shaderPath + fileName };
    FILE* file{ fopen(filePath.c_str(), "rb") };
    fseek(file, 0, SEEK_END);
    long fileSize{ ftell(file) };
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory* mem{ bgfx::alloc(fileSize + 1) };
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

int main() {
    glfwInit();
    GLFWwindow* window{ glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "bgfx adventures!", nullptr, nullptr) };
	bgfx::PlatformData pd;
	pd.nwh = glfwGetWin32Window(window);
	bgfx::setPlatformData(pd);
	
	bgfx::Init bgfxInit;
	bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
	bgfxInit.resolution.width = WINDOW_WIDTH;
	bgfxInit.resolution.height = WINDOW_HEIGHT;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	bgfx::init(bgfxInit);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    bgfx::VertexLayout pcvDecl;
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    bgfx::VertexBufferHandle vbh{ bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl) };
    bgfx::IndexBufferHandle ibh{ bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList))) };

    bgfx::ShaderHandle vsh{ loadShader("vs_cubes.bin") };
    bgfx::ShaderHandle fsh{ loadShader("fs_cubes.bin") };
    bgfx::ProgramHandle program{ bgfx::createProgram(vsh, fsh, true) };

    unsigned int counter{ 0 };
    while (true) {
        const bx::Vec3 at{ 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye{ 0.0f, 0.0f, -5.0f };
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
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

    bgfx::destroy(ibh);
    bgfx::destroy(vbh);
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}