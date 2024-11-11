#include "quick_sort.h"

void swap(int *arr, ssize_t i, ssize_t j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

void quick_sort(int *arr, ssize_t p, ssize_t q) {
  if (p >= q)
    return;
  ssize_t r = partition(arr, p, q);
  quick_sort(arr, p, r - 1);
  quick_sort(arr, r + 1, q);
}

ssize_t partition(int *arr, ssize_t p, ssize_t q) {
  int i = p - 1;
  for (ssize_t j = p; j <= q; j++) {
    if (arr[j] <= arr[q]) {
      swap(arr, ++i, j);
    }
  }
  return i;
}
