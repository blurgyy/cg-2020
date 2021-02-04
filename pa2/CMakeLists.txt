cmake_minimum_required(VERSION 3.19)
project("pbr") # Project name

set(target_name "pbr") # Target name (target can be an executable or a library)
set(sources "main.cpp") # Sources

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O2 -w -DNDEBUG")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -w -DNDEBUG -fopenmp")

# # Set this in Makefile and use `make debug|release` instead.
# # Default target in Makefile should be set to debug.
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

add_executable(${target_name} "${sources}")

# tinyobjloader
add_subdirectory(extern/tinyobjloader)
target_link_libraries(${target_name} tinyobjloader)

# Custom headers
include_directories(include extern)
add_subdirectory(include)
target_link_libraries(${target_name} wheels)

# vim: set ft=cmake:

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Jan 29 2021, 15:33 [CST]
