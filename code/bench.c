#include "allocator.h"
#include "buffer_allocator.h"
#include "counting_sort.h"
#include "heap_sort.h"
#include "math.h"
#include "quick_sort.h"
#include "quick_sort_3_way.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static inline int int_get_key(const void *a) { return *(const int *)a; }

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
    fprintf(stderr, "Usage: %s <length> <max_value> <n_runs>\n", argv[0]);
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
  double *quick_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);
  double *counting_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);
  double *quick_sort_3way_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);
  double *heap_sort_times =
      std_allocator.alloc(n_runs * sizeof(double), std_allocator.state);

  for (int r = 0; r < n_runs; r++) {
    // Generate a random array of n integers in [1, m]
    int *array = std_allocator.alloc(n * sizeof(int), std_allocator.state);
    for (int i = 0; i < n; i++) {
      array[i] = rand() % m + 1;
    }

    // Quick Sort
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

      quick_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }

    // Counting Sort
    {
      size_t buffer_size = n * sizeof(int)    // output array
                           + m * sizeof(int); // count array
      allocator_t buffer_allocator = buffer_allocator_init(buffer_size);

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        int *out_array =
            buffer_allocator.alloc(n * sizeof(int), buffer_allocator.state);

        counting_sort(array, n, m, sizeof(int), out_array, int_get_key,
                      buffer_allocator);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      counting_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }

    // Quick Sort 3-way
    {
      allocator_t buffer_allocator = buffer_allocator_init(n * sizeof(int));

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        int *array_copy =
            buffer_allocator.alloc(n * sizeof(int), buffer_allocator.state);
        memcpy(array_copy, array, n * sizeof(int));

        quick_sort_3_way(array_copy, n, sizeof(int), cmp_int);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      quick_sort_3way_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }

    // Heap Sort
    {
      allocator_t buffer_allocator = buffer_allocator_init(n * sizeof(int));

      int count = 0;
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (1) {
        int *array_copy =
            buffer_allocator.alloc(n * sizeof(int), buffer_allocator.state);
        memcpy(array_copy, array, n * sizeof(int));

        heap_sort(array_copy, n, sizeof(int), cmp_int, std_allocator);

        count++;
        clock_gettime(CLOCK_MONOTONIC, &end);

        if (elapsed_seconds(start, end) >= min_time)
          break;

        buffer_allocator_reset(buffer_allocator);
      }

      heap_sort_times[r] = elapsed_seconds(start, end) / count;

      buffer_allocator_fini(buffer_allocator);
    }
    std_allocator.dealloc(array, std_allocator.state);
  }

  // Calcolo tempo medio e deviazione standard
  double quick_sort_avg_time = 0, counting_sort_avg_time = 0,
         quick_sort_3way_avg_time = 0, heap_sort_avg_time = 0;
  for (int r = 0; r < n_runs; r++) {
    quick_sort_avg_time += quick_sort_times[r];
    counting_sort_avg_time += counting_sort_times[r];
    quick_sort_3way_avg_time += quick_sort_3way_times[r];
    heap_sort_avg_time += heap_sort_times[r];
  }
  quick_sort_avg_time /= n_runs;
  counting_sort_avg_time /= n_runs;
  quick_sort_3way_avg_time /= n_runs;
  heap_sort_avg_time /= n_runs;

  double quick_sort_std_dev = 0, counting_sort_std_dev = 0,
         quick_sort_3way_std_dev = 0, heap_sort_std_dev = 0;
  for (int r = 0; r < n_runs; r++) {
    quick_sort_std_dev += pow(quick_sort_times[r] - quick_sort_avg_time, 2);
    counting_sort_std_dev +=
        pow(counting_sort_times[r] - counting_sort_avg_time, 2);
    quick_sort_3way_std_dev +=
        pow(quick_sort_3way_times[r] - quick_sort_3way_avg_time, 2);
    heap_sort_std_dev += pow(heap_sort_times[r] - heap_sort_avg_time, 2);
  }
  quick_sort_std_dev = sqrt(quick_sort_std_dev / n_runs);
  counting_sort_std_dev = sqrt(counting_sort_std_dev / n_runs);
  quick_sort_3way_std_dev = sqrt(quick_sort_3way_std_dev / n_runs);
  heap_sort_std_dev = sqrt(heap_sort_std_dev / n_runs);

  // Print risultati
  printf("QUICK SORT\tCOUNTING SORT\tQUICK SORT 3-WAY\tHEAP SORT\n");
  printf("%.15f\t%.15f\t%.15f\t%.15f\n", quick_sort_avg_time,
         counting_sort_avg_time, quick_sort_3way_avg_time, heap_sort_avg_time);
  printf("%.15f\t%.15f\t%.15f\t%.15f\n", quick_sort_std_dev,
         counting_sort_std_dev, quick_sort_3way_std_dev, heap_sort_std_dev);

  std_allocator.dealloc(quick_sort_times, std_allocator.state);
  std_allocator.dealloc(counting_sort_times, std_allocator.state);
  std_allocator.dealloc(quick_sort_3way_times, std_allocator.state);
  std_allocator.dealloc(heap_sort_times, std_allocator.state);

  return 0;
}
