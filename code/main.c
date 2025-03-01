#include "allocator.h"
#include "counting_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "max_heap.h"
#include "merge_sort.h"
#include "quick_sort.h"
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
    max_heap_init(&max_heap, 100, allocator);

    arr_el_t el = {.key = 7, .data = NULL};
    max_heap_insert(&max_heap, el);
    arr_el_t max = max_heap_get_max(&max_heap);
    printf("added %d, max: %d\n", el.key, max.key);
    max_heap_print(&max_heap);

    arr_el_t el2 = {.key = 9, .data = NULL};
    max_heap_insert(&max_heap, el2);
    max = max_heap_get_max(&max_heap);
    printf("added %d, max: %d\n", el2.key, max.key);
    max_heap_print(&max_heap);

    arr_el_t el3 = {.key = 2, .data = NULL};
    max_heap_insert(&max_heap, el3);
    max = max_heap_get_max(&max_heap);
    printf("added %d, max: %d\n", el3.key, max.key);
    max_heap_print(&max_heap);

    max = max_heap_extract_max(&max_heap);
    printf("extracted max: %d\n", max.key);
    max_heap_print(&max_heap);

    max = max_heap_extract_max(&max_heap);
    printf("extracted max: %d\n", max.key);
    max_heap_print(&max_heap);

    max_heap_fini(&max_heap);
    printf("\n");
  }

  {
    max_heap_t max_heap;

    arr_el_t *els = allocator.alloc(5 * sizeof(arr_el_t), allocator.state);
    els[0].key = 5;
    els[1].key = 2;
    els[2].key = 4;
    els[3].key = 6;
    els[4].key = 1;

    max_heap_init(&max_heap, 5, allocator);
    max_heap_build(&max_heap, els, 5);
    /*max_heap_print(&max_heap);*/

    max_heap_fini(&max_heap);
    allocator.dealloc(els, allocator.state);
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
    arr_el_t array[] = {{5, NULL}, {2, NULL}, {4, NULL},
                        {6, NULL}, {1, NULL}, {3, NULL}};
    size_t n = sizeof(array) / sizeof(array[0]);
    heap_sort(array, n, allocator);
    for (size_t i = 0; i < n; i++) {
      printf("%d ", array[i].key);
    }
    printf("\n");
    printf("\n");
  }

  {
    printf("Counting sort\n");
    arr_el_t array[] = {{5, NULL}, {2, NULL}, {4, NULL},
                        {6, NULL}, {1, NULL}, {3, NULL}};
    size_t n = sizeof(array) / sizeof(array[0]);

    arr_el_t *out = allocator.alloc(n * sizeof(arr_el_t), allocator.state);
    counting_sort(array, n, 6, out, allocator);
    for (size_t i = 0; i < n; i++) {
      printf("%d ", out[i].key);
    }
    printf("\n");
    allocator.dealloc(out, allocator.state);
    printf("\n");
  }

  return 0;
}
