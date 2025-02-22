#include "heap_sort.h"
#include <string.h>

void heap_sort(max_heap_el_t *arr, size_t n, allocator_t allocator) {
  max_heap_t max_heap;
  max_heap_init(&max_heap, n, allocator);
  max_heap_build(&max_heap, arr, n);
  for (size_t i = max_heap.n_items - 1; i > 0; i--) {
    max_heap_el_t tmp = max_heap.items[0];
    max_heap.items[0] = max_heap.items[i];
    max_heap.items[i] = tmp;

    max_heap.n_items--;
    max_heap_heapify(&max_heap, 0);
  }

  // Copy the sorted array back to the input array.
  for (size_t i = 0; i < n; i++) {
    arr[i] = max_heap.items[i];
  }

  max_heap_fini(&max_heap);
}
