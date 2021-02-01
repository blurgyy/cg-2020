cmake_minimum_required(VERSION 3.19)
project("wheels") # Project name

set(target_name "wheels") # Target name (target can be an executable or a library)
set(sources
    Camera.cpp
    Scene.cpp
    Screen.cpp
    Triangle.cpp
    global.cpp
) # Sources

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O2 -w -DNDEBUG")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -w -DNDEBUG")

# # Build an executable:
# add_executable(${target_name} ${sources})
# Or, build a library to be linked:
add_library(${target_name} STATIC ${sources})
# add_library(${target_name} SHARED ${sources})
# add_library(${target_name} MODULE ${sources})  # I have no idea what this is

# vim: set ft=cmake:

# Author: Blurgy <gy@blurgy.xyz>
# Date:   Jan 30 2021, 00:52 [CST]
