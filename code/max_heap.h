#ifndef __H_MAX_HEAP_
#define __H_MAX_HEAP_
#include "allocator.h"
#include "shared.h"
#include <stddef.h>

typedef struct {
  void *items;
  size_t el_size;
  size_t n_items;
  size_t capacity;
  get_key get_key;
  allocator_t allocator;
} max_heap_t;

void max_heap_init(max_heap_t *heap, size_t capacity, size_t el_size,
                   get_key get_key, allocator_t allocator);

int max_heap_build(max_heap_t *heap, void *els, size_t n);

void *max_heap_get_max(max_heap_t *heap);

void *max_heap_extract_max(max_heap_t *heap);

/*void max_heap_increase_priority(max_heap_t *heap, size_t i, int inc);*/

/*void max_heap_decrease_priority(max_heap_t *heap, size_t i, int dec);*/

void max_heap_heapify(max_heap_t *heap, size_t i);

int max_heap_insert(max_heap_t *heap, void *el);

void max_heap_fini(max_heap_t *heap);

void max_heap_print(max_heap_t *heap);

#endif // !__H_MAX_HEAP_
