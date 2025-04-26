#include "allocator.h"
#include "counting_sort.h"
#include "heap.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "quick_sort_3_way.h"
#include <stdio.h>

static inline int int_get_key(const void *a) { return *(const int *)a; }

int cmp_int(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x > y) - (x < y);
}

int cmp_int_rev(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x < y) - (x > y);
}

void print_array(int *array, size_t n) {
  for (size_t i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

void heap_print(heap_t *heap) {
  char *items = heap->items;
  for (size_t i = 0; i < heap->n_items; i++) {
    printf("%d ", *(int *)(items + i * heap->el_size));
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
    heap_t heap;
    void *items = allocator.alloc(5 * sizeof(int), allocator.state);
    heap_init(&heap, items, 5, sizeof(int), cmp_int);

    heap_insert(&heap, &(int){5});
    int *max = heap_get_max(&heap);
    printf("added %d, max: %d\n", 5, *max);
    heap_print(&heap);

    heap_insert(&heap, &(int){9});
    max = heap_get_max(&heap);
    printf("added %d, max: %d\n", 9, *max);
    heap_print(&heap);

    heap_insert(&heap, &(int){2});
    max = heap_get_max(&heap);
    printf("added %d, max: %d\n", 2, *max);
    heap_print(&heap);

    max = heap_extract(&heap);
    printf("extracted max: %d\n", *max);
    heap_print(&heap);

    max = heap_extract(&heap);
    printf("extracted max: %d\n", *max);
    heap_print(&heap);
    printf("\n");

    allocator.dealloc(items, allocator.state);
  }

  {
    heap_t heap;

    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);

    heap_init(&heap, array, 6, sizeof(int), cmp_int);
    heap_build(&heap, array, n, n, sizeof(int), cmp_int);
    heap_print(&heap);
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
    heap_sort(array, n, sizeof(int), cmp_int);
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
    counting_sort(array, n, 7, sizeof(int), out, int_get_key, allocator);
    for (size_t i = 0; i < n; i++) {
      printf("%d ", out[i]);
    }
    printf("\n");
    allocator.dealloc(out, allocator.state);
    printf("\n");
  }

  {
    printf("Quick sort 3-way\n");
    int array[] = {5, 2, 4, 6, 1, 3};
    size_t n = sizeof(array) / sizeof(array[0]);
    quick_sort_3_way(array, n, sizeof(int), cmp_int);
    print_array(array, n);
    printf("\n");
  }

  return 0;
}
