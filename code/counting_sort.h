#ifndef __H_COUNTING_SORT_
#define __H_COUNTING_SORT_
#include "allocator.h"
#include "shared.h"
#include <stddef.h>

void counting_sort(arr_el_t *arr, size_t n, size_t k, arr_el_t *out,
                   allocator_t allocator);

#endif // !__H_COUNTING_SORT_
