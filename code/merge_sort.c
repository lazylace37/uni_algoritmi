#include "merge_sort.h"
#include <string.h>

void _merge(void *_arr, size_t p, size_t r, size_t q, size_t el_size, cmp_f cmp,
            allocator_t allocator) {
  char *arr = (char *)_arr;

  size_t len = q - p + 1;
  size_t i = p;
  size_t j = r + 1;

  char *tmp = allocator.alloc(len * el_size, allocator.state);
  for (size_t k = 0; k < len; k++) {
    if (i <= r && j <= q) {
      if (cmp(arr + i * el_size, arr + j * el_size) < 0) {
        memcpy(tmp + k * el_size, arr + i * el_size, el_size);
        i++;
      } else {
        memcpy(tmp + k * el_size, arr + j * el_size, el_size);
        j++;
      }
    } else if (j > q) {
      memcpy(tmp + k * el_size, arr + i * el_size, el_size);
      i++;
    } else {
      memcpy(tmp + k * el_size, arr + j * el_size, el_size);
      j++;
    }
  }

  memcpy(arr + p * el_size, tmp, len * el_size);

  allocator.dealloc(tmp, allocator.state);
}

void _merge_sort(void *arr, size_t p, size_t q, size_t el_size, cmp_f cmp,
                 allocator_t allocator) {
  if (p < q) {
    size_t r = (p + q) / 2;
    _merge_sort(arr, p, r, el_size, cmp, allocator);
    _merge_sort(arr, r + 1, q, el_size, cmp, allocator);
    _merge(arr, p, r, q, el_size, cmp, allocator);
  }
}

void merge_sort(void *arr, size_t n, size_t el_size, cmp_f cmp,
                allocator_t allocator) {
  _merge_sort(arr, 0, n - 1, el_size, cmp, allocator);
}
