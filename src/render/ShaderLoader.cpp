#include <cassert>

#include "ShaderLoader.h"

namespace render {

bgfx::ShaderHandle loadShader(std::string const &fileName) {
  // we only support direct3D11 or 12 since it's the target developping platform
  // supporting other platforms should be quite straightforwards,
  // just replicate the shaders for the other platforms and make a switch case
  // here
  assert(bgfx::getRendererType() == bgfx::RendererType::Direct3D12 ||
         bgfx::getRendererType() == bgfx::RendererType::Direct3D11);
  std::string filePath{"shaders/" + fileName};

  FILE *file{nullptr};
  fopen_s(&file, filePath.c_str(), "rb");
  fseek(file, 0, SEEK_END);
  long fileSize{ftell(file)};
  fseek(file, 0, SEEK_SET);

  const bgfx::Memory *mem{bgfx::alloc(fileSize + 1)};
  fread(mem->data, 1, fileSize, file);
  mem->data[mem->size - 1] = '\0';
  fclose(file);

  return bgfx::createShader(mem);
}

} // namespace render
