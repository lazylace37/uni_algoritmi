#include "quick_sort.h"
#include "shared.h"
#include "swap.h"
#include <stdio.h>
#include <string.h>

static size_t partition(void *_array, size_t p, size_t q, size_t el_size,
                        cmp_f cmp) {
  char *arr_ptr = (char *)_array;

  size_t i = p;
  char x[el_size];
  memcpy(x, arr_ptr + q * el_size, el_size);

  for (size_t j = p; j < q; j++) {
    if (cmp(arr_ptr + j * el_size, x) <= 0) {
      swap(arr_ptr + i * el_size, arr_ptr + j * el_size, el_size);
      i++;
    }
  }

  swap(arr_ptr + i * el_size, arr_ptr + q * el_size, el_size);

  return i;
}

static void _quick_sort(void *array, size_t p, size_t q, size_t el_size,
                        cmp_f cmp) {
  if (p < q) {
    size_t r = partition(array, p, q, el_size, cmp);
    if (r > 0)
      _quick_sort(array, p, r - 1, el_size, cmp);
    _quick_sort(array, r + 1, q, el_size, cmp);
  }
}

void quick_sort(void *array, size_t n, size_t el_size, cmp_f cmp) {
  _quick_sort(array, 0, n - 1, el_size, cmp);
}
