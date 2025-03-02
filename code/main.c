#include "allocator.h"
#include "counting_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "max_heap.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include <stdio.h>

static inline int int_get_key(const void *a) { return *(const int *)a; }

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

int main(void) {
  {
    printf("Insertion sort\n");
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    insertion_sort(array, n, sizeof(array[0]), cmp_int);
    print_array(array, n);
    printf("\n");
  }

  allocator_t allocator = get_default_allocator();
  {
    printf("Merge sort\n");
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    merge_sort(array, n, sizeof(array[0]), cmp_int, allocator);
    print_array(array, n);
    printf("\n");
  }

  {
    printf("Max heap\n");
    max_heap_t max_heap;
    max_heap_init(&max_heap, 5, sizeof(int), int_get_key, allocator);

    max_heap_insert(&max_heap, &(int){5});
    int *max = max_heap_get_max(&max_heap);
    printf("added %d, max: %d\n", 5, *max);
    max_heap_print(&max_heap);

    max_heap_insert(&max_heap, &(int){9});
    max = max_heap_get_max(&max_heap);
    printf("added %d, max: %d\n", 9, *max);
    max_heap_print(&max_heap);

    max_heap_insert(&max_heap, &(int){2});
    max = max_heap_get_max(&max_heap);
    printf("added %d, max: %d\n", 2, *max);
    max_heap_print(&max_heap);

    max = max_heap_extract_max(&max_heap);
    printf("extracted max: %d\n", *max);
    max_heap_print(&max_heap);

    max = max_heap_extract_max(&max_heap);
    printf("extracted max: %d\n", *max);
    max_heap_print(&max_heap);

    max_heap_fini(&max_heap);
    printf("\n");
  }

  {
    max_heap_t max_heap;

    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);

    max_heap_init(&max_heap, 5, sizeof(int), int_get_key, allocator);
    max_heap_build(&max_heap, array, n);
    max_heap_print(&max_heap);
    max_heap_fini(&max_heap);
  }

  {
    printf("Quick sort\n");
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    quick_sort(array, n, sizeof(int), cmp_int);
    print_array(array, n);
    printf("\n");
  }

  {
    printf("Heap sort\n");
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    heap_sort(array, n, sizeof(int), int_get_key, allocator);
    for (size_t i = 0; i < n; i++) {
      printf("%d ", array[i]);
    }
    printf("\n");
    printf("\n");
  }

  {
    printf("Counting sort\n");
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);

    int *out = allocator.alloc(n * sizeof(int), allocator.state);
    counting_sort(array, n, 6, sizeof(int), out, int_get_key, allocator);
    for (size_t i = 0; i < n; i++) {
      printf("%d ", out[i]);
    }
    printf("\n");
    allocator.dealloc(out, allocator.state);
    printf("\n");
  }

  return 0;
}
