#ifndef __H_HEAP_
#define __H_HEAP_
#include "allocator.h"
#include "shared.h"
#include <stddef.h>

typedef struct {
  void *items;
  size_t el_size;
  size_t n_items;
  size_t capacity;
  cmp_f cmp;
} heap_t;

void heap_init(heap_t *heap, void *items, size_t capacity, size_t el_size,
               cmp_f cmp);

int heap_build(heap_t *heap, void *items, size_t n, size_t capacity,
               size_t el_size, cmp_f cmp);

void *heap_get_max(heap_t *heap);

void *heap_extract(heap_t *heap);

/*void heap_increase_priority(heap_t *heap, size_t i, int inc);*/

/*void heap_decrease_priority(heap_t *heap, size_t i, int dec);*/

void heap_heapify(heap_t *heap, size_t i);

int heap_insert(heap_t *heap, void *el);

#endif // !__H_HEAP_
