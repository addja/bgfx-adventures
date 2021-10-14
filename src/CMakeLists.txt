cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(main)

add_subdirectory(render)
add_subdirectory(job)

add_executable(main "main.cpp")

target_link_libraries(main
  PRIVATE
    bgfx
    bx
    glfw
    render
    job
  )

# ---------------
# Shader handling
# ---------------

set(SHADERS
    "shaders/fs_cubes.bin"
    "shaders/vs_cubes.bin")

# Get the output directory for executables. By using the property of the target, we do not need to
# distinguish different platforms.
get_target_property(RUNTIME_OUTPUT main RUNTIME_OUTPUT_DIRECTORY)

# Copy shader files into bin/shaders
foreach(shader IN LISTS SHADERS)
    add_custom_command(
        OUTPUT ${RUNTIME_OUTPUT}/${shader}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${shader}
            ${RUNTIME_OUTPUT}/${shader}
        MAIN_DEPENDENCY ${shader})

    # Add copied shader file to private source file of the target
    target_sources(main PRIVATE ${RUNTIME_OUTPUT}/${shader})
endforeach()
    
