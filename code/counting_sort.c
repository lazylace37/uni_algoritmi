#include "counting_sort.h"
#include <stdio.h>

void counting_sort(arr_el_t *arr, size_t n, size_t k, arr_el_t *out,
                   allocator_t allocator) {
  int *c = allocator.alloc((k + 1) * sizeof(int), allocator.state);
  for (size_t j = 0; j < k + 1; j++) { // Θ(k)
    c[j] = 0;
  }
  for (size_t i = 0; i < n; i++) { // Θ(n)
    c[arr[i].key]++;
  }
  for (size_t j = 1; j < k + 1; j++) { // Θ(k)
    c[j] += c[j - 1];
  }
  for (ssize_t i = n - 1; i >= 0; i--) { // Θ(n)
    out[c[arr[i].key] - 1] = arr[i];
    c[arr[i].key]--;
  }
  allocator.dealloc(c, allocator.state);
}
