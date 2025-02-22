#include "max_heap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static inline size_t parent(size_t i) { return i / 2; }
static inline size_t left(size_t i) { return 2 * i + 1; }
static inline size_t right(size_t i) { return 2 * i + 2; }

void max_heap_init(max_heap_t *heap, size_t capacity, allocator_t allocator) {
  heap->items =
      allocator.alloc(capacity * sizeof(max_heap_el_t), allocator.state);
  heap->n_items = 0;
  heap->capacity = capacity;
  heap->allocator = allocator;
}

int max_heap_build(max_heap_t *heap, max_heap_el_t *els, size_t n) {
  assert(heap != NULL);
  assert(heap->n_items == 0);
  assert(els != NULL);
  if (n > heap->capacity) {
    return 1;
  }

  heap->n_items = n;
  for (size_t i = 0; i < n; i++) {
    heap->items[i] = els[i];
  }

  for (ssize_t i = n / 2; i >= 0; i--) {
    max_heap_heapify(heap, i);
  }
  return 0;
}

max_heap_el_t max_heap_get_max(max_heap_t *heap) {
  assert(heap != NULL);
  assert(heap->n_items > 0);
  return heap->items[0];
}

max_heap_el_t max_heap_extract_max(max_heap_t *heap) {
  assert(heap != NULL);
  assert(heap->n_items > 0);

  // Swap the first element with the last element.
  max_heap_el_t tmp = heap->items[0];
  heap->items[0] = heap->items[heap->n_items - 1];
  heap->items[heap->n_items - 1] = tmp;

  heap->n_items--;

  if (heap->n_items > 1) // Restore the heap property.
    max_heap_heapify(heap, 0);
  return heap->items[heap->n_items];
}

void max_heap_increase_priority(max_heap_t *heap, size_t i, int inc) {
  assert(heap != NULL);
  assert(i < heap->n_items);

  heap->items[i].key += inc;

  while (i > 0 && heap->items[i].key > heap->items[parent(i)].key) {
    max_heap_el_t tmp = heap->items[i];
    heap->items[i] = heap->items[parent(i)];
    heap->items[parent(i)] = tmp;

    i = parent(i);
  }
}
void max_heap_decrease_priority(max_heap_t *heap, size_t i, int dec) {
  assert(heap != NULL);
  assert(i < heap->n_items);

  heap->items[i].key -= dec;
  max_heap_heapify(heap, i);
}

void max_heap_heapify(max_heap_t *heap, size_t i) {
  assert(heap != NULL);
  if (i >= heap->n_items) {
    return;
  }

  size_t l = left(i);
  size_t r = right(i);
  size_t m = i;

  if (l < heap->n_items && heap->items[l].key > heap->items[i].key) {
    m = l;
  }
  if (r < heap->n_items && heap->items[r].key > heap->items[m].key) {
    m = r;
  }

  if (m != i) {
    // Swap the two elements.
    max_heap_el_t tmp = heap->items[i];
    heap->items[i] = heap->items[m];
    heap->items[m] = tmp;

    max_heap_heapify(heap, m);
  }
}

int max_heap_insert(max_heap_t *heap, max_heap_el_t el) {
  assert(heap != NULL);
  if (heap->n_items >= heap->capacity) {
    return 1;
  }

  // Insert the element at the end of the heap.
  heap->items[heap->n_items] = el;
  heap->n_items++;

  // Swap with its parent until greater.
  size_t i = heap->n_items - 1;
  while (i > 0 && heap->items[i].key > heap->items[parent(i)].key) {
    // Swap the two elements.
    max_heap_el_t tmp = heap->items[i];
    heap->items[i] = heap->items[parent(i)];
    heap->items[parent(i)] = tmp;

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
    printf("%d ", heap->items[i].key);
  }
  printf("\n");
}
