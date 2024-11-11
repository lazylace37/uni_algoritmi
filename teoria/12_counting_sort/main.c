#include "counting_sort.h"
int main(void) {
  int arr[] = {1, 4, 1, 2, 7, 5, 2};
  int n = sizeof(arr) / sizeof(arr[0]);
  int output[n];
  counting_sort(arr, n, output, 7);

  for (int i = 0; i < n; i++) {
    printf("%d ", output[i]);
  }
  printf("\n");
  return 0;
}
