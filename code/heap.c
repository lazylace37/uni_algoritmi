#include "heap.h"
#include "shared.h"
#include "swap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static inline size_t parent(size_t i) { return i / 2; }
static inline size_t left(size_t i) { return 2 * i + 1; }
static inline size_t right(size_t i) { return 2 * i + 2; }

void heap_init(heap_t *heap, void *items, size_t capacity, size_t el_size,
               cmp_f cmp) {
  heap->items = items;
  heap->el_size = el_size;
  heap->n_items = 0;
  heap->capacity = capacity;
  heap->cmp = cmp;
}

int heap_build(heap_t *heap, void *items, size_t n, size_t capacity,
               size_t el_size, cmp_f cmp) {
  assert(heap != NULL);
  heap->items = items;
  heap->el_size = el_size;
  heap->n_items = n;
  heap->capacity = capacity;
  heap->cmp = cmp;

  for (ssize_t i = heap->n_items / 2; i >= 0; i--) {
    heap_heapify(heap, i);
  }
  return 0;
}

void *heap_get_max(heap_t *heap) {
  assert(heap != NULL);
  assert(heap->n_items > 0);
  return heap->items;
}

void *heap_extract(heap_t *heap) {
  assert(heap != NULL);
  assert(heap->n_items > 0);

  char *items = (char *)heap->items;
  swap(items, items + (heap->n_items - 1) * heap->el_size, heap->el_size);

  heap->n_items--;

  if (heap->n_items > 1) // Restore the heap property.
    heap_heapify(heap, 0);
  return items + heap->n_items * heap->el_size;
}

/*void heap_increase_priority(heap_t *heap, size_t i, int inc) {*/
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

/*void heap_decrease_priority(heap_t *heap, size_t i, int dec) {*/
/*  assert(heap != NULL);*/
/*  assert(i < heap->n_items);*/
/**/
/*  heap->items[i].key -= dec;*/
/*  heap_heapify(heap, i);*/
/*}*/

void heap_heapify(heap_t *heap, size_t i) {
  assert(heap != NULL);
  if (i >= heap->n_items) {
    return;
  }

  char *items = (char *)heap->items;

  size_t l = left(i);
  size_t r = right(i);
  size_t m = i;

  if (l < heap->n_items &&
      heap->cmp(items + l * heap->el_size, items + i * heap->el_size) > 0) {
    m = l;
  }
  if (r < heap->n_items &&
      heap->cmp(items + r * heap->el_size, items + m * heap->el_size) > 0) {
    m = r;
  }

  if (m != i) {
    // Swap the two elements.
    swap(items + i * heap->el_size, items + m * heap->el_size, heap->el_size);

    heap_heapify(heap, m);
  }
}

int heap_insert(heap_t *heap, void *el) {
  assert(heap != NULL);
  if (heap->n_items >= heap->capacity) {
    return 1;
  }

  char *items = (char *)heap->items;

  // Insert the element at the end of the heap.
  memcpy(items + heap->n_items * heap->el_size, el, heap->el_size);
  heap->n_items++;

  // Swap with its parent until greater.
  size_t i = heap->n_items - 1;
  while (i > 0 && heap->cmp(items + i * heap->el_size,
                            items + parent(i) * heap->el_size) > 0) {
    // Swap the two elements.
    swap(items + i * heap->el_size, items + parent(i) * heap->el_size,
         heap->el_size);

    i = parent(i);
  }
  return 0;
}
