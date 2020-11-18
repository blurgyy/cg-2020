cmake_minimum_required(VERSION 3.18)
project(oglt)

# OpenGL
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

add_executable(oglt main.cpp)
target_link_libraries(oglt ${OPENGL_LIBRARY} glfw GLEW)

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Nov 18 2020, 15:36 [CST]
