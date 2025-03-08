#include "heap_sort.h"
#include <string.h>

void heap_sort(void *arr, size_t n, size_t el_size, cmp cmp,
               allocator_t allocator) {
  heap_t heap;
  heap_init(&heap, n, el_size, cmp, allocator);
  heap_build(&heap, arr, n);
  char tmp[el_size];
  for (size_t i = heap.n_items - 1; i > 0; i--) {
    memcpy(tmp, heap.items, el_size);
    memcpy(heap.items, heap.items + i * heap.el_size, heap.el_size);
    memcpy(heap.items + i * heap.el_size, tmp, heap.el_size);

    heap.n_items--;
    heap_heapify(&heap, 0);
  }

  // Copy the sorted array back to the input array.
  for (size_t i = 0; i < n; i++) {
    memcpy(arr + i * el_size, heap.items + i * el_size, el_size);
  }

  heap_fini(&heap);
}
