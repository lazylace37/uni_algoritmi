#ifndef __H_QUICK_SORT__
#define __H_QUICK_SORT__
#include <stddef.h>
#include <stdio.h>

void quick_sort(int *arr, ssize_t p, ssize_t q);
ssize_t partition(int *arr, ssize_t p, ssize_t q);

#endif // !__H_QUICK_SORT__
