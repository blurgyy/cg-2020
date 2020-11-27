cmake_minimum_required(VERSION 3.18)
project(pa1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

add_executable(zbuffer main.cpp)

# Custom headers, include these lines **after** other packages
include_directories("include")
add_subdirectory("include")
target_link_libraries(zbuffer wheels)

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Nov 18 2020, 17:33 [CST]
