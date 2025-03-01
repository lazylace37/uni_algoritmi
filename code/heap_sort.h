#ifndef __H_HEAP_SORT__
#define __H_HEAP_SORT__
#include "allocator.h"
#include "max_heap.h"
#include <stddef.h>

void heap_sort(arr_el_t *arr, size_t n, allocator_t allocator);

#endif // !__H_HEAP_SORT__
