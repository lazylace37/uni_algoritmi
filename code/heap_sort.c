#include "heap_sort.h"
#include "swap.h"
#include <string.h>

void heap_sort(void *arr, size_t n, size_t el_size, cmp_f cmp) {
  heap_t heap = {0};
  heap_build(&heap, arr, n, n, el_size, cmp);

  char *items = (char *)heap.items;

  for (size_t i = heap.n_items - 1; i > 0; i--) {
    swap(items, items + i * heap.el_size, heap.el_size);

    heap.n_items--;
    heap_heapify(&heap, 0);
  }
}
