#ifndef __H_MAX_HEAP_
#define __H_MAX_HEAP_
#include "allocator.h"
#include "shared.h"
#include <stddef.h>

typedef struct {
  int key;
  void *data;
} max_heap_el_t;

typedef struct {
  max_heap_el_t *items;
  size_t n_items;
  size_t capacity;
  allocator_t allocator;
} max_heap_t;

void max_heap_init(max_heap_t *heap, size_t capacity, allocator_t allocator);

int max_heap_build(max_heap_t *heap, max_heap_el_t *els, size_t n);

max_heap_el_t max_heap_get_max(max_heap_t *heap);

max_heap_el_t max_heap_extract_max(max_heap_t *heap);

void max_heap_increase_priority(max_heap_t *heap, size_t i, int inc);

void max_heap_decrease_priority(max_heap_t *heap, size_t i, int dec);

void max_heap_heapify(max_heap_t *heap, size_t i);

int max_heap_insert(max_heap_t *heap, max_heap_el_t el);

void max_heap_fini(max_heap_t *heap);

void max_heap_print(max_heap_t *heap);

#endif // !__H_MAX_HEAP_
