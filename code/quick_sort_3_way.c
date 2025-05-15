#include "quick_sort_3_way.h"
#include "swap.h"
#include <stdio.h>
#include <string.h>

static void partition(void *_array, size_t p, size_t q, ssize_t *out_l,
                      ssize_t *out_r, size_t el_size, cmp_f cmp) {
  char *array = (char *)_array;

  char x[el_size];
  memcpy(x, array + q * el_size, el_size);

  ssize_t i = p, left = p, right = q;
  while (i <= right) {
    void *current = array + i * el_size;
    int cmp_result = cmp(current, &x);

    if (cmp_result < 0) {
      swap(array + i * el_size, array + left * el_size, el_size);
      i++;
      left++;
    } else if (cmp_result > 0) {
      swap(array + i * el_size, array + right * el_size, el_size);
      right--;
    } else {
      i++;
    }
  }

  *out_l = left;
  *out_r = right;
}

static void _quick_sort_3_way(void *array, ssize_t p, ssize_t q, size_t el_size,
                              cmp_f cmp) {
  if (p < q) {
    ssize_t l, r;
    partition(array, p, q, &l, &r, el_size, cmp);
    _quick_sort_3_way(array, p, l - 1, el_size, cmp);
    _quick_sort_3_way(array, r + 1, q, el_size, cmp);
  }
}

void quick_sort_3_way(void *array, size_t n, size_t el_size, cmp_f cmp) {
  _quick_sort_3_way(array, 0, n - 1, el_size, cmp);
}
