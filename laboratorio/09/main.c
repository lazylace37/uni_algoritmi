#include <stddef.h>
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

  int s;
  if (scanf("%d", &s) < 1) {
    printf("%d %d", -1, -1);
    return 1;
  }

#if VERSIONE == 1
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i; j < n; j++) {
      int sum = 0;
      for (size_t k = i; k <= j; k++) {
        sum += vec[k];
      }
      if (sum == s) {
        printf("%zu %zu", i, j);
        return 0;
      }
    }
  }
#endif /* if VERSIONE == 1 */

#if VERSIONE == 2
  int sum = 0;
  size_t i = 0;
  for (size_t j = 0; j < n; j++) {
    sum += vec[j];
    if (sum >= s) {
      while (sum > s) {
        sum -= vec[i];
        i++;
      }
      if (sum == s) {
        printf("%zu %zu", i, j);
        return 0;
      }
    }
  }
#endif /* if VERSIONE == 2 */

  printf("%d %d", -1, -1);
  return 1;
}
