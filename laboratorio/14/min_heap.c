#include "min_heap.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

static inline size_t parent(size_t i) { return (i - 1) / 2; }
static inline size_t left(size_t i) { return 2 * i + 1; }
static inline size_t right(size_t i) { return 2 * i + 2; }

void min_heap_heapify(min_heap_t *min_heap, size_t i) {
  if (i >= min_heap->length) {
    return;
  }

  size_t l = left(i);
  size_t r = right(i);
  size_t m = i;

  if (l < min_heap->length && min_heap->data[l] < min_heap->data[i]) {
    m = l;
  }
  if (r < min_heap->length && min_heap->data[r] < min_heap->data[m]) {
    m = r;
  }

  if (m != i) {
    int tmp = min_heap->data[i];
    min_heap->data[i] = min_heap->data[m];
    min_heap->data[m] = tmp;

    min_heap_heapify(min_heap, m);
  }
}

min_heap_t min_heap_build(int *arr, size_t n, size_t capacity) {
  min_heap_t min_heap = {
      .data = malloc(capacity * sizeof(int)),
      .length = n,
      .capacity = capacity,
  };

  for (size_t i = 0; i < n; i++) {
    min_heap.data[i] = arr[i];
  }

  // Heapify.
  for (ssize_t i = min_heap.length / 2; i >= 0; i--) {
    min_heap_heapify(&min_heap, i);
  }

  return min_heap;
}

size_t min_heap_length(min_heap_t *min_heap) { return min_heap->length; }

int min_heap_getmin(min_heap_t *min_heap) {
  if (min_heap->length == 0) {
    return -1;
  }
  return min_heap->data[0];
}

int min_heap_extract(min_heap_t *min_heap) {
  if (min_heap->length == 0) {
    return -1;
  }

  int min = min_heap->data[0];
  min_heap->data[0] = min_heap->data[min_heap->length - 1];
  min_heap->length--;

  min_heap_heapify(min_heap, 0);
  return min;
}

void min_heap_insert(min_heap_t *min_heap, int el) {
  if (min_heap->length >= min_heap->capacity) {
    return;
  }

  // Inserisci elemento alla fine dell'array.
  min_heap->data[min_heap->length] = el;
  min_heap->length++;

  int i = min_heap->length - 1;
  while (i > 0 && min_heap->data[i] < min_heap->data[parent(i)]) {
    int tmp = min_heap->data[i];
    min_heap->data[i] = min_heap->data[parent(i)];
    min_heap->data[parent(i)] = tmp;

    i = parent(i);
  }
}

void min_heap_change(min_heap_t *min_heap, size_t i, int el) {
  if (i >= min_heap->length) {
    return;
  }

  if (el < min_heap->data[i]) { // Diminuisci la chiave.
    min_heap->data[i] = el;
    while (i > 0 && min_heap->data[i] < min_heap->data[parent(i)]) {
      int tmp = min_heap->data[i];
      min_heap->data[i] = min_heap->data[parent(i)];
      min_heap->data[parent(i)] = tmp;

      i = parent(i);
    }
  } else { // Aumenta la chiave.
    min_heap->data[i] = el;
    min_heap_heapify(min_heap, i);
  }
}

void min_heap_print(min_heap_t *min_heap) {
  for (size_t i = 0; i < min_heap->length; i++) {
    printf("%d ", min_heap->data[i]);
  }
  printf("\n");
}
