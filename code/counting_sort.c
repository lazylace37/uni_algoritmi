#include "counting_sort.h"
#include <stdio.h>
#include <string.h>

void counting_sort(void *arr, size_t n, size_t k, size_t el_size, void *out,
                   get_key get_key, allocator_t allocator) {
  int *c = allocator.alloc((k + 1) * sizeof(int), allocator.state);
  for (size_t j = 0; j < k + 1; j++) { // Θ(k)
    c[j] = 0;
  }
  for (size_t i = 0; i < n; i++) { // Θ(n)
    int key = get_key(arr + i * el_size);
    c[key]++;
  }
  for (size_t j = 1; j < k + 1; j++) { // Θ(k)
    c[j] += c[j - 1];
  }
  for (ssize_t i = n - 1; i >= 0; i--) { // Θ(n)
    int key = get_key(arr + i * el_size);
    memcpy(out + (c[key] - 1) * el_size, arr + i * el_size, el_size);
    c[key]--;
  }
  allocator.dealloc(c, allocator.state);
}
