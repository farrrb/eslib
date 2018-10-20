////////////////////////////////////////////////////////////////////////////////
/// \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
///               [Released under MIT License. See LICENSE for details]
///
/// \file         esl_heap.c
/// \brief        A simple heap implementation.
////////////////////////////////////////////////////////////////////////////////
#include "esl_heap.h"

////////////////////////////////////////////////////////////////////////////////
/// Type Definitions
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
  bool     init;        ///< Initialization flag
  bool     enabled;     ///< Enabled Flag

  uint8_t *heap_start;  ///< Pointer to first position of the heap
  uint8_t *heap_end;    ///< Pointer to one element after the last usable
  uint8_t *heap_cur;    ///< Heap cursor

  size_t   size;        ///< Size of the heap
  uint8_t  alignment;   ///< Alignment
} EslHeapInternals_t;

//------------------------------------------------------------------------------
// Local Data
//------------------------------------------------------------------------------

EslHeapInternals_t EslHeap_self = {0};

//------------------------------------------------------------------------------
// External Functions
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
bool EslHeap_init(uint8_t *heap_start, size_t size, uint8_t alignment)
{
  // Guard Statement(s)
  // 1. Is the heap already initialized?
  if (EslHeap_self.init == true)
  {
    return false;
  }
  // 2. Is the input valid?
  if ((heap_start == NULL) || (size == 0) || (alignment == 0))
  {
    return false;
  }

  // Assign internal data
  EslHeap_self.heap_start = heap_start;
  EslHeap_self.heap_end   = heap_start + size;
  EslHeap_self.size       = size;
  EslHeap_self.alignment  = alignment;

  // Set up heap cursor
  EslHeap_self.heap_cur   = heap_start;
  // Align heap cursor (if not aligned already)
  if ((int)heap_start & (alignment - 1))
  {
    EslHeap_self.heap_cur += alignment - ((int)heap_start & (alignment - 1));
  }

  EslHeap_self.enabled = false;
  EslHeap_self.init    = true;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
bool EslHeap_enable(bool enable)
{
  if (EslHeap_self.init != true)
  {
    return false;
  }
  else
  {
    EslHeap_self.enabled = enable;
    return true;
  }
}

////////////////////////////////////////////////////////////////////////////////
size_t EslHeap_getAvailableMemory(void)
{
  if (EslHeap_self.init != true)
  {
    return 0;
  }

  return (EslHeap_self.heap_end - EslHeap_self.heap_cur);
}

////////////////////////////////////////////////////////////////////////////////
void * EslHeap_alloc(size_t size)
{
  void  *ret = NULL;

  size_t no_of_bytes = size;

  if ((EslHeap_self.init != true) || (EslHeap_self.enabled != true))
  {
    return NULL;
  }

  // Sometimes we need to allocate more bytes to meet alignment requirements
  if (size & (EslHeap_self.alignment - 1))
  {
    no_of_bytes += EslHeap_self.alignment - (size & (EslHeap_self.alignment - 1));
  }

  // acquire lock
  ESL_CFG_ACQUIRE_LOCK();

  if ((EslHeap_self.heap_cur + no_of_bytes) <= EslHeap_self.heap_end)
  {
    ret = EslHeap_self.heap_cur;
    EslHeap_self.heap_cur += no_of_bytes;
  }

  // release lock
  ESL_CFG_RELEASE_LOCK();

  return ret;
}
