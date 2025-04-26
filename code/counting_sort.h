#ifndef __H_COUNTING_SORT_
#define __H_COUNTING_SORT_
#include "allocator.h"
#include "shared.h"
#include <stddef.h>

void counting_sort(void *arr, size_t n, size_t k, size_t el_size, void *out,
                   get_key_f get_key, allocator_t allocator);

#endif // !__H_COUNTING_SORT_
