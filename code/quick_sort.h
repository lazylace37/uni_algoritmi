#ifndef __H_QUICK_SORT_
#define __H_QUICK_SORT_
#include "shared.h"
#include <stddef.h>

void quick_sort(void *array, size_t n, size_t el_size, cmp cmp);

#endif // !__H_QUICK_SORT_
