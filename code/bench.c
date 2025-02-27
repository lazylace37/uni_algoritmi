#include "allocator.h"
#include "buffer_allocator.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cmp_int(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x > y) - (x < y);
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: %s <length> <max_value> <n_runs>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  int m = atoi(argv[2]);
  int n_runs = atoi(argv[3]);

  allocator_t std_allocator = get_default_allocator();
  double *insertion_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);
  double *merge_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);
  double *heap_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);
  double *quick_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);

  for (int r = 0; r < n_runs; r++) {
    // Generate a random array of n integers between 0 and m
    int *array = std_allocator.alloc(n * sizeof(int), std_allocator.state);
    for (int i = 0; i < n; i++) {
      array[i] = rand() % m;
    }

    struct timespec start, end;
    {
      /*int *array_copy =*/
      /*    std_allocator.alloc(n * sizeof(int), std_allocator.state);*/
      /*for (int i = 0; i < n; i++)*/
      /*  array_copy[i] = array[i];*/
      /**/
      /*clock_gettime(CLOCK_MONOTONIC, &start);*/
      /*insertion_sort(array_copy, n, sizeof(int), cmp_int);*/
      /*clock_gettime(CLOCK_MONOTONIC, &end);*/
      /*double elapsed =*/
      /*    (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;*/
      /*insertion_sort_times[r] = elapsed;*/
      /**/
      /*std_allocator.dealloc(array_copy, std_allocator.state);*/
    }
    {
      int *array_copy =
          std_allocator.alloc(n * sizeof(int), std_allocator.state);
      for (int i = 0; i < n; i++)
        array_copy[i] = array[i];

      clock_gettime(CLOCK_MONOTONIC, &start);
      merge_sort(array_copy, n, sizeof(int), cmp_int, std_allocator);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      merge_sort_times[r] = elapsed;

      std_allocator.dealloc(array_copy, std_allocator.state);
    }
    {
      max_heap_el_t *heap_array =
          std_allocator.alloc(n * sizeof(max_heap_el_t), std_allocator.state);
      for (int i = 0; i < n; i++) {
        heap_array[i].key = array[i];
        heap_array[i].data = NULL;
      }

      clock_gettime(CLOCK_MONOTONIC, &start);
      heap_sort(heap_array, n, std_allocator);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      heap_sort_times[r] = elapsed;

      std_allocator.dealloc(heap_array, std_allocator.state);
    }
    {
      int *array_copy =
          std_allocator.alloc(n * sizeof(int), std_allocator.state);
      for (int i = 0; i < n; i++)
        array_copy[i] = array[i];

      clock_gettime(CLOCK_MONOTONIC, &start);
      quick_sort(array_copy, n, sizeof(int), cmp_int);
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      quick_sort_times[r] = elapsed;

      std_allocator.dealloc(array_copy, std_allocator.state);
    }

    std_allocator.dealloc(array, std_allocator.state);
  }

  double insertion_sort_time = 0, merge_sort_time = 0, heap_sort_time = 0,
         quick_sort_time = 0;
  for (int r = 0; r < n_runs; r++) {
    insertion_sort_time += insertion_sort_times[r];
    merge_sort_time += merge_sort_times[r];
    heap_sort_time += heap_sort_times[r];
    quick_sort_time += quick_sort_times[r];
  }

  insertion_sort_time /= n_runs;
  merge_sort_time /= n_runs;
  heap_sort_time /= n_runs;
  quick_sort_time /= n_runs;

  printf("%f\t%f\t%f\t%f\n", insertion_sort_time, merge_sort_time,
         heap_sort_time, quick_sort_time);

  return 0;
}
