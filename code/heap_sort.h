#ifndef __H_HEAP_SORT__
#define __H_HEAP_SORT__
#include "allocator.h"
#include "heap.h"
#include "shared.h"
#include <stddef.h>

void heap_sort(void *arr, size_t n, size_t el_size, cmp_f cmp,
               allocator_t allocator);

#endif // !__H_HEAP_SORT__
