#ifndef __H_INSERTION_SORT__
#define __H_INSERTION_SORT__
#include "shared.h"
#include <stddef.h>

void insertion_sort(void *array, size_t n, size_t el_size, cmp cmp);

#endif // !__H_INSERTION_SORT__
