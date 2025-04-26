#include <stdio.h>
#include <time.h>

static inline double duration(struct timespec start, struct timespec end) {
  return end.tv_sec - start.tv_sec +
         ((end.tv_nsec - start.tv_nsec) / (double)1000000000.0);
}

double getResolution(void) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  do {
    clock_gettime(CLOCK_MONOTONIC, &end);
  } while (duration(start, end) <= 1.0e-05f);
  return duration(start, end);
}

int main(void) {
  double resolution = getResolution();
  printf("System clock resolution: %.15f\n", resolution);
  return 0;
}
