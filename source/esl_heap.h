////////////////////////////////////////////////////////////////////////////////
/// \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
///               [Released under MIT License. See LICENSE for details]
///
/// \file         esl_heap.h
/// \brief        A simple heap implementation.
////////////////////////////////////////////////////////////////////////////////
#include "esl_internals.h"

////////////////////////////////////////////////////////////////////////////////
/// \brief      Initialization function
///
/// \param      heap_start  Pointer to start of heap
/// \param[in]  size        [byte] The size of the heap
/// \param[in]  alignment   The alignment of memory objects on the heap
///
/// \retval     true        Initialization was successful.
/// \retval     false       An error occurred.
////////////////////////////////////////////////////////////////////////////////
bool EslHeap_init(uint8_t *heap_start, size_t size, uint8_t alignment);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Heap enabling / disabling function
///
/// \param[in]  enable  (true := enable / false := disable)
///
/// \retval     true    Enable successful
/// \retval     false   Enable unsuccessful
////////////////////////////////////////////////////////////////////////////////
bool EslHeap_enable(bool enable);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Returns the amount of available memory
///
/// \return     [byte] Available memory for allocation
////////////////////////////////////////////////////////////////////////////////
size_t EslHeap_getAvailableMemory(void);

////////////////////////////////////////////////////////////////////////////////
/// \brief      The allocator function
///
/// \param[in]  size  [byte] The size of the memory block to allocate
///
/// \return     Pointer to the allocated memory block
/// \retval     NULL Invalid memory block / an error occured
////////////////////////////////////////////////////////////////////////////////
void * EslHeap_alloc(size_t size);