#include <stdio.h>
#include <string.h>

#define VERSIONE 4

#if VERSIONE == 2
void swap(int *arr, ssize_t i, ssize_t j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

ssize_t partition(int *arr, ssize_t p, ssize_t q) {
  int i = p - 1;
  for (ssize_t j = p; j <= q; j++) {
    if (arr[j] <= arr[q]) {
      swap(arr, ++i, j);
    }
  }
  return i;
}

void quick_sort(int *arr, ssize_t p, ssize_t q) {
  if (p >= q)
    return;
  ssize_t r = partition(arr, p, q);
  quick_sort(arr, p, r - 1);
  quick_sort(arr, r + 1, q);
}
#endif /* if VERSIONE == 2 */

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
    printf("No majority\n");
    return 1;
  }
  if (n == 1) {
    printf("%d\n", vec[0]);
    return 0;
  }

#if VERSIONE == 1 // O(n^2)
  for (size_t i = 0; i < n; i++) {
    size_t count = 1;
    for (size_t j = i + 1; j < n; j++) {
      if (vec[i] == vec[j])
        count++;
    }
    if (count > n / 2) {
      printf("%d\n", vec[i]);
      return 0;
    }
  }
#endif /* if VERSIONE == 1 */

#if VERSIONE == 2
  quick_sort(vec, 0, n - 1); // O(n log n) almeno nel caso medio, forse era
                             // meglio usare heap sort per garantire O(n log n)

  int el = vec[0];
  size_t c = 1;
  for (size_t i = 1; i < n; i++) { // O(n)
    if (vec[i] != el) {
      el = vec[i];
      c = 1;
    } else
      c++;

    if (c > n / 2) {
      printf("%d\n", vec[i]);
      return 0;
    }
  }
#endif /* if VERSIONE == 2 */

#if VERSIONE == 3
  size_t k = 100;
  int s[k] = {};
  for (size_t i = 0; i < n; i++) {
    s[vec[i]]++;
  }

  int maj_idx = 0;
  for (size_t i = 1; i < k; i++) {
    if (s[i] > s[maj_idx])
      maj_idx = i;
  }

  if (s[maj_idx] > (int)(n / 2)) {
    printf("%d\n", maj_idx);
    return 0;
  }
#endif /* if VERSIONE == 3 */

#if VERSIONE == 4
  int cnt = 0, maj_candidate = vec[0];
  for (size_t i = 0; i < n; i++) {
    if (cnt == 0) {
      maj_candidate = vec[i];
      cnt = 1;
    } else if (vec[i] == maj_candidate) {
      cnt++;
    } else {
      cnt--;
    }
  }
  int ver_cnt = 0;
  for (size_t i = 0; i < n; i++) {
    if (vec[i] == maj_candidate)
      ver_cnt++;
  }
  if (ver_cnt > n / 2) {
    printf("%d\n", maj_candidate);
    return 0;
  }
#endif /* if VERSIONE == 4 */

  printf("No majority\n");
  return 1;
}
