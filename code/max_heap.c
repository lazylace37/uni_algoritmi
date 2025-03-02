#include "max_heap.h"
#include "shared.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static inline size_t parent(size_t i) { return i / 2; }
static inline size_t left(size_t i) { return 2 * i + 1; }
static inline size_t right(size_t i) { return 2 * i + 2; }

void max_heap_init(max_heap_t *heap, size_t capacity, size_t el_size,
                   get_key get_key, allocator_t allocator) {
  heap->items = allocator.alloc(capacity * el_size, allocator.state);
  heap->el_size = el_size;
  heap->n_items = 0;
  heap->capacity = capacity;
  heap->get_key = get_key;
  heap->allocator = allocator;
}

int max_heap_build(max_heap_t *heap, void *els, size_t n) {
  assert(heap != NULL);
  assert(heap->n_items == 0);
  assert(els != NULL);
  if (n > heap->capacity) {
    return 1;
  }

  heap->n_items = n;
  for (size_t i = 0; i < n; i++) {
    memcpy(heap->items + i * heap->el_size, els + i * heap->el_size,
           heap->el_size);
  }

  for (ssize_t i = n / 2; i >= 0; i--) {
    max_heap_heapify(heap, i);
  }
  return 0;
}

void *max_heap_get_max(max_heap_t *heap) {
  assert(heap != NULL);
  assert(heap->n_items > 0);
  return heap->items;
}

void *max_heap_extract_max(max_heap_t *heap) {
  assert(heap != NULL);
  assert(heap->n_items > 0);

  // Swap the first element with the last element.
  char tmp[heap->el_size];
  memcpy(tmp, heap->items, heap->el_size);
  memcpy(heap->items, heap->items + (heap->n_items - 1) * heap->el_size,
         heap->el_size);
  memcpy(heap->items + (heap->n_items - 1) * heap->el_size, tmp, heap->el_size);

  heap->n_items--;

  if (heap->n_items > 1) // Restore the heap property.
    max_heap_heapify(heap, 0);
  return heap->items + heap->n_items * heap->el_size;
}

/*void max_heap_increase_priority(max_heap_t *heap, size_t i, int inc) {*/
/*  assert(heap != NULL);*/
/*  assert(i < heap->n_items);*/
/**/
/*  heap->items[i].key += inc;*/
/**/
/*  while (i > 0 && heap->items[i].key > heap->items[parent(i)].key) {*/
/*    arr_el_t tmp = heap->items[i];*/
/*    heap->items[i] = heap->items[parent(i)];*/
/*    heap->items[parent(i)] = tmp;*/
/**/
/*    i = parent(i);*/
/*  }*/
/*}*/

/*void max_heap_decrease_priority(max_heap_t *heap, size_t i, int dec) {*/
/*  assert(heap != NULL);*/
/*  assert(i < heap->n_items);*/
/**/
/*  heap->items[i].key -= dec;*/
/*  max_heap_heapify(heap, i);*/
/*}*/

void max_heap_heapify(max_heap_t *heap, size_t i) {
  assert(heap != NULL);
  if (i >= heap->n_items) {
    return;
  }

  size_t l = left(i);
  size_t r = right(i);
  size_t m = i;

  if (l < heap->n_items && heap->get_key(heap->items + l * heap->el_size) >
                               heap->get_key(heap->items + i * heap->el_size)) {
    m = l;
  }
  if (r < heap->n_items && heap->get_key(heap->items + r * heap->el_size) >
                               heap->get_key(heap->items + m * heap->el_size)) {
    m = r;
  }

  char tmp[heap->el_size];
  if (m != i) {
    // Swap the two elements.
    memcpy(tmp, heap->items + i * heap->el_size, heap->el_size);
    memcpy(heap->items + i * heap->el_size, heap->items + m * heap->el_size,
           heap->el_size);
    memcpy(heap->items + m * heap->el_size, tmp, heap->el_size);

    max_heap_heapify(heap, m);
  }
}

int max_heap_insert(max_heap_t *heap, void *el) {
  assert(heap != NULL);
  if (heap->n_items >= heap->capacity) {
    return 1;
  }

  // Insert the element at the end of the heap.
  memcpy(heap->items + heap->n_items * heap->el_size, el, heap->el_size);
  heap->n_items++;

  // Swap with its parent until greater.
  size_t i = heap->n_items - 1;
  char tmp[heap->el_size];
  while (i > 0 && heap->get_key(heap->items + i * heap->el_size) >
                      heap->get_key(heap->items + parent(i) * heap->el_size)) {
    // Swap the two elements.
    memcpy(tmp, heap->items + i * heap->el_size, heap->el_size);
    memcpy(heap->items + i * heap->el_size,
           heap->items + parent(i) * heap->el_size, heap->el_size);
    memcpy(heap->items + parent(i) * heap->el_size, tmp, heap->el_size);

    i = parent(i);
  }
  return 0;
}

void max_heap_fini(max_heap_t *heap) {
  assert(heap != NULL);
  heap->allocator.dealloc(heap->items, heap->allocator.state);
}

void max_heap_print(max_heap_t *heap) {
  assert(heap != NULL);
  for (size_t i = 0; i < heap->n_items; i++) {
    printf("%d ", heap->get_key(heap->items + i * heap->el_size));
  }
  printf("\n");
}
