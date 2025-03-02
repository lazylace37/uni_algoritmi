#ifndef __H_HEAP_SORT__
#define __H_HEAP_SORT__
#include "allocator.h"
#include "max_heap.h"
#include <stddef.h>

void heap_sort(void *arr, size_t n, size_t el_size, get_key get_key,
               allocator_t allocator);

#endif // !__H_HEAP_SORT__
