cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(main)

add_subdirectory(render)

add_executable(main "main.cpp")

target_link_libraries(main
  PRIVATE
    bgfx
    bx
    glfw
    render
  )
