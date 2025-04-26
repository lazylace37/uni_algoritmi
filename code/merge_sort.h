#ifndef __H_MERGE_SORT__
#define __H_MERGE_SORT__
#include "allocator.h"
#include "shared.h"
#include <stddef.h>

void merge_sort(void *_array, size_t n, size_t el_size, cmp_f cmp,
                allocator_t allocator);

#endif // !__H_MERGE_SORT__
