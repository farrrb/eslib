################################################################################
# \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
#               [Released under MIT License. See LICENSE for details]
################################################################################

enable_testing()

add_executable(test_esl_heap test_esl_heap.c)
target_link_libraries(test_esl_heap unity eslib)

add_test(NAME test_esl_heap COMMAND test_esl_heap)
