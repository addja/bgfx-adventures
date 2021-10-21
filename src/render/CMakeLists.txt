cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(render LANGUAGES CXX)

add_library(render
    STATIC
     "ShaderLoader.cpp"
     "ShaderLoader.h"
     "ExampleCube.h")

target_link_libraries(render
  PRIVATE
    bgfx
  )
