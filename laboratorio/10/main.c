#include <limits.h>
#include <stdio.h>
#include <string.h>

#define VERSIONE 2

void parse_stdin(int vec[], size_t vec_size, size_t *n) {
  char input[1 << 12];
  fgets(input, sizeof input, stdin);
  input[strlen(input) - 1] = '\0';

  *n = 0;
  int bytes_read = 0, offset = 0;
  while (*n < vec_size &&
         sscanf(input + offset, "%d%n", &vec[*n], &bytes_read) > 0) {
    (*n)++;
    offset += bytes_read;
  }
}

int main(void) {
  int vec[256];
  size_t n;
  parse_stdin(vec, sizeof vec / sizeof vec[0], &n);
  if (n <= 0) {
    printf("%d %d", -1, -1);
    return 1;
  }
  if (n <= 1) {
    printf("%d %d", 0, 0);
    return 0;
  }

#if VERSIONE == 1
  int max_difference = INT_MIN;
  size_t min_i = -1, min_j = -1;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i; j < n; j++) {
      int diff = vec[j] - vec[i];
      if (diff > max_difference) {
        max_difference = diff;
        min_i = i;
        min_j = j;
      }
    }
  }
  printf("%zu %zu\n", min_i, min_j);
#endif /* if VERSIONE == 1 */

#if VERSIONE == 2
  /*int max_difference = 0, diff = 0;*/
  /*ssize_t max_difference_i = 0, max_difference_j = -1, diff_i = 0;*/
  /*for (size_t i = 1; i < n; i++) {*/
  /*  if (vec[i - 1] > vec[i]) {*/
  /*    if (diff > max_difference) {*/
  /*      max_difference = diff;*/
  /*      max_difference_i = diff_i;*/
  /*      max_difference_j = i - 1;*/
  /*    }*/
  /*    diff = 0;*/
  /*    diff_i = i;*/
  /*  } else {*/
  /*    diff += vec[i] - vec[i - 1];*/
  /*    printf("diff: %d\n", diff);*/
  /*  }*/
  /*}*/
  /*if (diff > max_difference) {*/
  /*  max_difference_i = diff_i;*/
  /*  max_difference_j = n - 1;*/
  /*}*/
  /*printf("%zu %zu\n", max_difference_i, max_difference_j);*/

  size_t max_difference_i = 0, max_difference_j = 0;
  size_t l_i = 0;
  int min = INT_MAX, diff = 0;
  for (size_t i = 0; i < n; i++) {
    if (min > vec[i]) {
      l_i = i; // Punta all'elemento minimo attuale
      min = vec[i];
    }
    if (diff < vec[i] - min) {
      diff = vec[i] - min;
      max_difference_i = l_i; // Punta all'elemento minimo attuale
      max_difference_j = i;   // Punta all'elemento massimo attuale
    }
  }
  printf("%zu %zu\n", max_difference_i, max_difference_j);
#endif /* if VERSIONE == 2 */
}
