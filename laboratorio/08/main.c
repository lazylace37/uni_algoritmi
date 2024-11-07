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
  if (n <= 0)
    return 0;

  int s;
  scanf("%d", &s);

#if VERSIONE == 1 // O(n^2)
  for (size_t i = 0; i < n - 1; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (vec[i] + vec[j] == s) {
        printf("%zu %zu", i, j);
        return 0;
      }
    }
  }
#endif /* if VERSIONE == 1 */

#if VERSIONE == 2 // O(n logn)
  for (size_t i = 0; i < n - 1; i++) {
    size_t p = i + 1, q = n - 1;
    do {
      size_t m = (p + q) / 2;
      int sum = vec[i] + vec[m];

      if (sum == s) {
        printf("%zu %zu", i, m);
        return 0;
      } else if (sum > s) {
        q = m - 1;
      } else {
        p = m + 1;
      }
    } while (p <= q);
  }
#endif /* if VERSIONE == 2 // O(n logn) */

  printf("%d %d", -1, -1);
  return 1;
}
