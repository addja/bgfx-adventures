#pragma once

#include <string>

#include "bgfx/bgfx.h"

namespace render {

bgfx::ShaderHandle loadShader(std::string const &fileName);

} // namespace render
