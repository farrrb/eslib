################################################################################
# \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
#               [Released under MIT License. See LICENSE for details]
################################################################################

add_library(eslib STATIC esl_heap.c)
target_include_directories(eslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
