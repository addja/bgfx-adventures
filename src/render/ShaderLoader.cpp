#include "ShaderLoader.h"
#include <cassert>
#include <fstream>

namespace render {

bgfx::ShaderHandle loadShader(std::string const &fileName) {
  // we only support direct3D11 or 12 since it's the target developping platform
  // supporting other platforms should be quite straightforwards,
  // just replicate the shaders for the other platforms and make a switch case
  // here
  assert(bgfx::getRendererType() == bgfx::RendererType::Direct3D12 ||
         bgfx::getRendererType() == bgfx::RendererType::Direct3D11);
  std::string filePath{"shaders/" + fileName};

  std::ifstream file(filePath, std::ios::binary);
  assert(file.is_open()); // failed to open file

  auto begin{file.tellg()};
  file.seekg(0, std::ios::end);
  auto end = file.tellg();
  long long fileSize{end - begin};

  file.seekg(0, std::ios::beg); // set the input position indicator
  const bgfx::Memory *mem{bgfx::alloc(fileSize + 1)};
  file.read(reinterpret_cast<char *>(mem->data), fileSize);
  mem->data[mem->size - 1] = '\0';

  return bgfx::createShader(mem);
}

} // namespace render
