################################################################################
# \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
#               [Released under MIT License. See LICENSE for details]
################################################################################
cmake_minimum_required(VERSION 3.9)

project(EmbeddedSystemsLibrary C)

if (CMAKE_COMPILER_IS_GNU)
  add_compile_options(-std=c99 -Wall -Wextra -Wpedantic)
endif()

# Libraries
add_subdirectory(externals/unity)
add_subdirectory(source)

# Unit Tests
add_subdirectory(test)
