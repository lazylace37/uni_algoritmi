#ifndef __H_COUNTING_SORT__
#define __H_COUNTING_SORT__
#include <stddef.h>
#include <stdio.h>

void counting_sort(int *arr, size_t n, int *out_arr, size_t k) {
  int c[k + 1];
  for (size_t j = 0; j < k + 1; j++) { // Θ(k)
    c[j] = 0;
  }
  for (size_t i = 0; i < n; i++) { // Θ(n)
    c[arr[i]] += 1;
  }
  for (size_t j = 1; j < k + 1; j++) { // Θ(k)
    c[j] += c[j - 1];
  }
  for (ssize_t i = n - 1; i >= 0; i--) {
    out_arr[c[arr[i]] - 1] = arr[i];
    c[arr[i]]--;
  }
}

#endif // !__H_COUNTING_SORT__
