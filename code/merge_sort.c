#include "merge_sort.h"
#include <string.h>

void _merge(void *_array, size_t p, size_t r, size_t q, size_t el_size, cmp cmp,
            allocator_t allocator) {
  size_t len = q - p + 1;
  size_t i = p;
  size_t j = r + 1;

  void *tmp = allocator.alloc(len * el_size, allocator.state);
  for (size_t k = 0; k < len; k++) {
    if (i <= r && j <= q) {
      if (cmp(_array + i * el_size, _array + j * el_size) < 0) {
        memcpy(tmp + k * el_size, _array + i * el_size, el_size);
        i++;
      } else {
        memcpy(tmp + k * el_size, _array + j * el_size, el_size);
        j++;
      }
    } else if (j > q) {
      memcpy(tmp + k * el_size, _array + i * el_size, el_size);
      i++;
    } else {
      memcpy(tmp + k * el_size, _array + j * el_size, el_size);
      j++;
    }
  }

  memcpy(_array + p * el_size, tmp, len * el_size);

  allocator.dealloc(tmp, allocator.state);
}

void _merge_sort(void *_array, size_t p, size_t q, size_t el_size, cmp cmp,
                 allocator_t allocator) {
  if (p < q) {
    size_t r = (p + q) / 2;
    _merge_sort(_array, p, r, el_size, cmp, allocator);
    _merge_sort(_array, r + 1, q, el_size, cmp, allocator);
    _merge(_array, p, r, q, el_size, cmp, allocator);
  }
}

void merge_sort(void *_array, size_t n, size_t el_size, cmp cmp,
                allocator_t allocator) {
  _merge_sort(_array, 0, n - 1, el_size, cmp, allocator);
}
