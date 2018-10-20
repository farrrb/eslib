################################################################################
# \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
#               [Released under MIT License. See LICENSE for details]
################################################################################

add_library(unity STATIC unity.c)
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
