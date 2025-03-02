#include "heap_sort.h"
#include <string.h>

void heap_sort(void *arr, size_t n, size_t el_size, get_key get_key,
               allocator_t allocator) {
  max_heap_t max_heap;
  max_heap_init(&max_heap, n, el_size, get_key, allocator);
  max_heap_build(&max_heap, arr, n);
  char tmp[el_size];
  for (size_t i = max_heap.n_items - 1; i > 0; i--) {
    memcpy(tmp, max_heap.items, el_size);
    memcpy(max_heap.items, max_heap.items + i * max_heap.el_size,
           max_heap.el_size);
    memcpy(max_heap.items + i * max_heap.el_size, tmp, max_heap.el_size);

    max_heap.n_items--;
    max_heap_heapify(&max_heap, 0);
  }

  // Copy the sorted array back to the input array.
  for (size_t i = 0; i < n; i++) {
    memcpy(arr + i * el_size, max_heap.items + i * el_size, el_size);
  }

  max_heap_fini(&max_heap);
}
