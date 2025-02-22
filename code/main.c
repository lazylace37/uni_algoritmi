#include "allocator.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include <stdio.h>

int cmp_int(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x > y) - (x < y);
}

void print_array(int *array, size_t n) {
  for (size_t i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  {
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    insertion_sort(array, n, sizeof(array[0]), cmp_int);
    print_array(array, n);
  }

  allocator_t allocator = get_default_allocator();
  {
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    merge_sort(array, n, sizeof(array[0]), cmp_int, allocator);
    print_array(array, n);
  }

  return 0;
}
