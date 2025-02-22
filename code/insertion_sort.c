#include "insertion_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(void *_array, size_t n, size_t el_size, cmp cmp) {
  char *arr_ptr = (char *)_array;

  char key[el_size]; // You won't convince me this is a bad idea.
  for (size_t i = 1; i < n; i++) {
    memcpy(key, arr_ptr + i * el_size, el_size);

    ssize_t j = i - 1;
    while (j >= 0 && cmp(arr_ptr + j * el_size, key) > 0) {
      memcpy(arr_ptr + (j + 1) * el_size, arr_ptr + j * el_size, el_size);
      j--;
    }
    memcpy(arr_ptr + (j + 1) * el_size, key, el_size);
  }
}
