#include "buffer_allocator.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmp_int(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x > y) - (x < y);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <length>\n", argv[0]);
    return 1;
  }

  int N_RUNS = 4;
  double insertion_sort_times[N_RUNS];
  double merge_sort_times[N_RUNS];
  double heap_sort_times[N_RUNS];
  double quick_sort_times[N_RUNS];

  int n = atoi(argv[1]);

  for (int r = 0; r < N_RUNS; r++) {
    allocator_t allocator = buffer_allocator_init(n * sizeof(int) * 100);

    int *array = allocator.alloc(n * sizeof(int), allocator.state);
    for (int i = 0; i < n; i++) {
      array[i] = rand();
    }

    struct timespec start, end;
    if (n < (1 << 10)) {
      int *array_copy = allocator.alloc(n * sizeof(int), allocator.state);
      for (int i = 0; i < n; i++)
        array_copy[i] = array[i];

      clock_gettime(CLOCK_MONOTONIC, &start);
      insertion_sort(array_copy, n, sizeof(int), cmp_int);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      insertion_sort_times[r] = elapsed;

      allocator.dealloc(array_copy, allocator.state);
    }
    {
      int *array_copy = allocator.alloc(n * sizeof(int), allocator.state);
      for (int i = 0; i < n; i++)
        array_copy[i] = array[i];

      clock_gettime(CLOCK_MONOTONIC, &start);
      merge_sort(array_copy, n, sizeof(int), cmp_int, allocator);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      merge_sort_times[r] = elapsed;

      allocator.dealloc(array_copy, allocator.state);
    }
    {
      max_heap_el_t *heap_array =
          allocator.alloc(n * sizeof(max_heap_el_t), allocator.state);
      for (int i = 0; i < n; i++) {
        heap_array[i].key = array[i];
        heap_array[i].data = NULL;
      }

      clock_gettime(CLOCK_MONOTONIC, &start);
      heap_sort(heap_array, n, allocator);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      heap_sort_times[r] = elapsed;

      allocator.dealloc(heap_array, allocator.state);
    }
    {
      int *array_copy = allocator.alloc(n * sizeof(int), allocator.state);
      for (int i = 0; i < n; i++)
        array_copy[i] = array[i];

      clock_gettime(CLOCK_MONOTONIC, &start);
      quick_sort(array_copy, n, sizeof(int), cmp_int);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      quick_sort_times[r] = elapsed;

      allocator.dealloc(array_copy, allocator.state);
    }

    allocator.dealloc(array, allocator.state);
    buffer_allocator_fini(allocator);
  }

  double insertion_sort_time = 0;
  for (int r = 0; r < N_RUNS; r++)
    insertion_sort_time += insertion_sort_times[r];
  insertion_sort_time /= N_RUNS;

  double merge_sort_time = 0;
  for (int r = 0; r < N_RUNS; r++)
    merge_sort_time += merge_sort_times[r];
  merge_sort_time /= N_RUNS;

  double heap_sort_time = 0;
  for (int r = 0; r < N_RUNS; r++)
    heap_sort_time += heap_sort_times[r];
  heap_sort_time /= N_RUNS;

  double quick_sort_time = 0;
  for (int r = 0; r < N_RUNS; r++)
    quick_sort_time += quick_sort_times[r];
  quick_sort_time /= N_RUNS;

  printf("%f\t%f\t%f\t%f\n", insertion_sort_time, merge_sort_time,
         heap_sort_time, quick_sort_time);

  return 0;
}
