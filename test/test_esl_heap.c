////////////////////////////////////////////////////////////////////////////////
/// \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
///               [Released under MIT License. See LICENSE for details]
///
/// \file         test_esl_heap.c
/// \brief        Test for esl_heap.c
////////////////////////////////////////////////////////////////////////////////
#include "unity.h"    // Test Framework
#include "esl_heap.h" // Test Candidate

#ifdef __GNUC__
  #define ALIGNED(n) __attribute__ ((aligned (n)))
#else
  #define ALIGNED(n)
#endif

#define ALIGNMENT 8
#define HEAP_SIZE 32

ALIGNED(ALIGNMENT) uint8_t heap_buffer[HEAP_SIZE] = {0};

typedef struct
{
  bool     init;
  bool     enabled;

  uint8_t *heap_start;
  uint8_t *heap_end;
  uint8_t *heap_cur;

  size_t   size;
  uint8_t  alignment;
} EslHeapInternals_t;

extern EslHeapInternals_t EslHeap_self;

void setUp(void)
{
  // Reset initialization flag
  EslHeap_self.init = false;
}

void tearDown(void)
{
}

void test_SETUP(void)
{
  // We have to make sure that heap_buffer meets our alignment constraints
  TEST_ASSERT_EQUAL_PTR(NULL, (void *)((int)&heap_buffer[0] & (ALIGNMENT - 1)));
}

void test_pre_init(void)
{
  // Pre-Initialization
  TEST_ASSERT_FALSE(EslHeap_self.init);
  TEST_ASSERT_FALSE(EslHeap_self.enabled);
  TEST_ASSERT_EQUAL_PTR(NULL, EslHeap_self.heap_cur);
  TEST_ASSERT_EQUAL_PTR(NULL, EslHeap_self.heap_start);
  TEST_ASSERT_EQUAL_PTR(NULL, EslHeap_self.heap_end);
  TEST_ASSERT_EQUAL_INT(0, EslHeap_self.size);
  TEST_ASSERT_EQUAL_UINT8(0, EslHeap_self.alignment);
  TEST_ASSERT_EQUAL((size_t)0, EslHeap_getAvailableMemory());
}

void test_init_fail(void)
{
  // Initialization should fail, because of invalid parameters
  TEST_ASSERT_FALSE(EslHeap_init(NULL, HEAP_SIZE, 1));
  TEST_ASSERT_FALSE(EslHeap_init(heap_buffer, 0, 1));
  TEST_ASSERT_FALSE(EslHeap_init(heap_buffer, HEAP_SIZE, 0));
}

void test_init_success(void)
{
  // Initialization
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));

  // Post-Initialization
  TEST_ASSERT_EQUAL((size_t)HEAP_SIZE, EslHeap_getAvailableMemory());
  TEST_ASSERT_TRUE(EslHeap_self.init);
  TEST_ASSERT_FALSE(EslHeap_self.enabled);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0],         EslHeap_self.heap_cur);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0],         EslHeap_self.heap_start);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[HEAP_SIZE], EslHeap_self.heap_end);
  TEST_ASSERT_EQUAL_INT(HEAP_SIZE, EslHeap_self.size);
  TEST_ASSERT_EQUAL_UINT8(1, EslHeap_self.alignment);
}

void test_enable(void)
{
  TEST_ASSERT_FALSE(EslHeap_self.enabled);
  TEST_ASSERT_FALSE(EslHeap_enable(true));

  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_FALSE(EslHeap_self.enabled);
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_TRUE(EslHeap_self.enabled);
}

void test_disable(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_FALSE(EslHeap_self.enabled);

  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_TRUE(EslHeap_self.enabled);

  TEST_ASSERT_TRUE(EslHeap_enable(false));
  TEST_ASSERT_FALSE(EslHeap_self.enabled);

  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_TRUE(EslHeap_self.enabled);
}

void test_alloc_1_byte(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0], EslHeap_alloc(1));
  TEST_ASSERT_EQUAL((size_t)(HEAP_SIZE - 1), EslHeap_getAvailableMemory());
}

void test_alloc_2_bytes(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0], EslHeap_alloc(2));
  TEST_ASSERT_EQUAL((size_t)(HEAP_SIZE - 2), EslHeap_getAvailableMemory());
}

void test_alloc_all_bytes_at_once(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0], EslHeap_alloc(HEAP_SIZE));
  TEST_ASSERT_EQUAL((size_t)0, EslHeap_getAvailableMemory());
}

void test_alloc_too_many(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL_PTR(NULL,            EslHeap_alloc(HEAP_SIZE+1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0], EslHeap_alloc(HEAP_SIZE));
}

void test_alloc_2_bytes_in_a_row(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0], EslHeap_alloc(1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[1], EslHeap_alloc(1));
  TEST_ASSERT_EQUAL((size_t)(HEAP_SIZE - 2), EslHeap_getAvailableMemory());
}

void test_alloc_multiple_bytes_in_a_row(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(heap_buffer, HEAP_SIZE, 1));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[0],  EslHeap_alloc(1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[1],  EslHeap_alloc(4));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[5],  EslHeap_alloc(6));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[11], EslHeap_alloc(13));
  TEST_ASSERT_EQUAL_PTR(NULL,             EslHeap_alloc(HEAP_SIZE));
}

void test_align_2bytes(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(&heap_buffer[1], (HEAP_SIZE - 1), 2));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL((size_t)(HEAP_SIZE - 2), EslHeap_getAvailableMemory());

  TEST_ASSERT_EQUAL_PTR(&heap_buffer[1],          EslHeap_self.heap_start);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[HEAP_SIZE],  EslHeap_self.heap_end);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[2],          EslHeap_self.heap_cur);

  TEST_ASSERT_EQUAL_PTR(&heap_buffer[2],  EslHeap_alloc(1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[4],  EslHeap_alloc(1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[6],  EslHeap_alloc(1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[8],  EslHeap_alloc(3));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[12], EslHeap_alloc(4));
}

void test_align_4bytes(void)
{
  TEST_ASSERT_TRUE(EslHeap_init(&heap_buffer[2], (HEAP_SIZE - 2), 4));
  TEST_ASSERT_TRUE(EslHeap_enable(true));
  TEST_ASSERT_EQUAL((size_t)(HEAP_SIZE - 4), EslHeap_getAvailableMemory());

  TEST_ASSERT_EQUAL_PTR(&heap_buffer[2],          EslHeap_self.heap_start);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[HEAP_SIZE],  EslHeap_self.heap_end);
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[4],          EslHeap_self.heap_cur);

  TEST_ASSERT_EQUAL_PTR(&heap_buffer[4],  EslHeap_alloc(1));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[8],  EslHeap_alloc(2));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[12], EslHeap_alloc(3));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[16], EslHeap_alloc(4));
  TEST_ASSERT_EQUAL_PTR(&heap_buffer[20], EslHeap_alloc(12));
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_SETUP);
  RUN_TEST(test_pre_init);
  RUN_TEST(test_init_fail);
  RUN_TEST(test_init_success);
  RUN_TEST(test_enable);
  RUN_TEST(test_disable);
  RUN_TEST(test_alloc_1_byte);
  RUN_TEST(test_alloc_2_bytes);
  RUN_TEST(test_alloc_all_bytes_at_once);
  RUN_TEST(test_alloc_too_many);
  RUN_TEST(test_alloc_2_bytes_in_a_row);
  RUN_TEST(test_alloc_multiple_bytes_in_a_row);
  RUN_TEST(test_align_2bytes);
  RUN_TEST(test_align_4bytes);

  return UNITY_END();
}
