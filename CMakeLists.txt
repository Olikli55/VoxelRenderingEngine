cmake_minimum_required(VERSION 3.20)
project(VoxelRenderingEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/Libraries")

# All project sources + headers (so CLion indexes them as part of the target)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/*.c"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(VoxelRenderingEngine ${PROJECT_SOURCES})

target_include_directories(VoxelRenderingEngine PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/src/render"
        "${CMAKE_SOURCE_DIR}/src/render/gl"
        "${CMAKE_SOURCE_DIR}/src/render/renderer"
        "${CMAKE_SOURCE_DIR}/src/world/core"
        "${CMAKE_SOURCE_DIR}/src/world/spatial"
        "${LIBRARIES_DIR}/include"
)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

target_link_libraries(VoxelRenderingEngine PRIVATE
        OpenGL::GL
        glfw
        dl
        pthread
)