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

double elapsed_seconds(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) +
         ((end.tv_nsec - start.tv_nsec) / (double)1e9);
}
double clock_resolution() {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  do {
    clock_gettime(CLOCK_MONOTONIC, &end);
  } while (elapsed_seconds(start, end) == 0.0);
  return elapsed_seconds(start, end);
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: %s <length> <max_value> <n_runs>\n", argv[0]);
    return 1;
  }

  // Calcola il tempo minimo misurabile
  double clock_res = clock_resolution();
  double max_relative_error = 0.001;
  double min_time = clock_res * (1 / max_relative_error + 1);

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

  // Generate a random array of n integers between 0 and m
  int *array = std_allocator.alloc(n * sizeof(int), std_allocator.state);
  for (int i = 0; i < n; i++) {
    array[i] = rand() % m;
  }

  // Run n_runs times on the same array
  for (int r = 0; r < n_runs; r++) {
    {
      allocator_t buffer_allocator = buffer_allocator_init(n * sizeof(int));

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        int *array_copy =
            buffer_allocator.alloc(n * sizeof(int), buffer_allocator.state);
        memcpy(array_copy, array, n * sizeof(int));

        insertion_sort(array_copy, n, sizeof(int), cmp_int);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      insertion_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }

    {
      allocator_t buffer_allocator = buffer_allocator_init(n * sizeof(int));

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        int *array_copy =
            buffer_allocator.alloc(n * sizeof(int), buffer_allocator.state);
        memcpy(array_copy, array, n * sizeof(int));

        merge_sort(array_copy, n, sizeof(int), cmp_int, std_allocator);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      merge_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }

    {
      allocator_t buffer_allocator =
          buffer_allocator_init(n * sizeof(max_heap_el_t));

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        max_heap_el_t *heap_array = buffer_allocator.alloc(
            n * sizeof(max_heap_el_t), buffer_allocator.state);
        for (int i = 0; i < n; i++) {
          heap_array[i].key = array[i];
          heap_array[i].data = NULL;
        }

        heap_sort(heap_array, n, std_allocator);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      heap_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }

    {
      allocator_t buffer_allocator = buffer_allocator_init(n * sizeof(int));

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        int *array_copy =
            buffer_allocator.alloc(n * sizeof(int), buffer_allocator.state);
        memcpy(array_copy, array, n * sizeof(int));

        quick_sort(array_copy, n, sizeof(int), cmp_int);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      heap_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }
  }
  std_allocator.dealloc(array, std_allocator.state);

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

  printf("INSERTION\tMERGE\tHEAP\tQUICK\n");
  printf("%f\t%f\t%f\t%f\n", insertion_sort_time, merge_sort_time,
         heap_sort_time, quick_sort_time);

  return 0;
}
