#include "quick_sort.h"
#include <stdio.h>

int main(void) {
  int arr[] = {5, 3, 8, 4, 2, 7, 1, 10};
  int n = sizeof(arr) / sizeof(arr[0]);
  quick_sort(arr, 0, n - 1);
  for (int i = 0; i < n; i++)
    printf("%d ", arr[i]);
  printf("\n");
  return 0;
}
