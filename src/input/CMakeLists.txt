cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(input)

add_library(input
    STATIC
     "Manager.h"
     "Job.h"
     "Job.cpp"
     "Input.h")

target_link_libraries(render
  PRIVATE
    glfw
  )
