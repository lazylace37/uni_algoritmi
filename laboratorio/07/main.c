#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define QUESITO 3

#define P 2147483647

#define left(n) (2 * n + 1)
#define right(n) (2 * n + 2)
#define min(a, b) (a <= b ? a : b)
#define max(a, b) (a >= b ? a : b)

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

int somma_intervallo(int v[], size_t n, size_t i, size_t j) {
  assert(j < n);
  int sum = 0;
  for (size_t k = i; k <= j; k++)
    sum += v[k];
  return sum;
}

void build_segment_tree_max(int *segment_tree, int *vec, size_t i, size_t p,
                            size_t q) {
  if (p >= q) {
    segment_tree[i] = vec[p];
  } else {
    size_t m = (p + q) / 2;
    build_segment_tree_max(segment_tree, vec, left(i), p, m);
    build_segment_tree_max(segment_tree, vec, right(i), m + 1, q);

    int l_val = segment_tree[left(i)];
    int r_val = segment_tree[right(i)];
    segment_tree[i] = max(l_val, r_val);
  }
}

int get_max(int *segment_tree, size_t i, size_t node_l, size_t node_r, size_t p,
            size_t q) {
  if (p >= q)
    return 0;
  if (node_l == p && node_r == q)
    return segment_tree[i];

  size_t node_m = (node_l + node_r) / 2;
  int l_val = get_max(segment_tree, left(i), node_l, node_m, p, node_m);
  int r_val =
      get_max(segment_tree, right(i), node_m + 1, node_r, node_m + 1, q);
  return max(l_val, r_val);
}

// x^n % mod = {
//  x * (x^2 % mod)^(n-1)/2 if n is odd
//  (x^2 % mod)^(n/2) if n is even
// }
uint64_t fast_exp_mod(uint64_t x, uint64_t n, uint64_t mod) {
  uint64_t result = 1;
  while (n > 0) {
    if (n % 2 == 1) {
      result = (result * x) % mod;
    }
    x = (x * x) % mod;
    n /= 2;
  }
  return result;
}

int main(void) {
  int vec[256];
  size_t n;
  parse_stdin(vec, sizeof vec / sizeof vec[0], &n);
  if (n <= 0)
    return 0;

  int idxs[256];
  size_t m;
  parse_stdin(idxs, sizeof idxs / sizeof idxs[0], &m);
  if (m % 2 != 0) {
    fprintf(stderr, "Didn't provide an even number of indexes\n");
    return 1;
  }

#if QUESITO == 1 // somma intervallo
  // Versione 1
  // for (size_t k = 0; k < m - 1; k += 2) { // Θ(m)
  //   int i = idxs[k];
  //   int j = idxs[k + 1];
  //   int sum = somma_intervallo(vec, n, i, j); // Θ(n)
  //   printf("%d ", sum);
  // }
  // printf("\n");

  // Versione 2
  int partial_sums[256] = {0};      // 0 0+vec[0] 0+vec[0]+vec[1] ....
  for (size_t i = 1; i <= n; i++) { // Θ(n)
    partial_sums[i] = partial_sums[i - 1] + vec[i - 1];
  }
  for (size_t k = 0; k < m - 1; k += 2) { // Θ(m)
    int i = idxs[k];                      // vec[k-1]
    int j = idxs[k + 1] + 1;              // Offset the second index
    int sum = partial_sums[j] - partial_sums[i];
    printf("%d ", sum);
  }
  printf("\n");
#endif /* if QUESITO == 1 */

#if QUESITO == 2 // max intervallo
  int segment_tree[4 * n];
  build_segment_tree_max(segment_tree, vec, 0, 0, n - 1); // Θ(n)
  for (size_t k = 0; k < m - 1; k += 2) {                 // Θ(m)
    int i = idxs[k];
    int j = idxs[k + 1];
    int max = get_max(segment_tree, 0, 0, n - 1, i, j);
    printf("%d ", max);
  }
  printf("\n");
#endif /* if QUESITO == 2 */

#if QUESITO == 3 // prodotto intervallo
  uint64_t partial_prod[256] = {};
  int zeros[256] = {0};
  for (size_t i = 0; i < n; i++) { // Θ(n)
    if (i > 0) {
      zeros[i] = zeros[i - 1] + (vec[i] == 0);

      int norm = vec[i] == 0 ? 1 : vec[i];
      partial_prod[i] = ((uint64_t)partial_prod[i - 1] * norm) % P;
    } else {
      partial_prod[i] = vec[i];
    }
  }
  for (size_t k = 0; k < m - 1; k += 2) { // Θ(m)
    int i = idxs[k];
    int j = idxs[k + 1] + 1;

    uint64_t prod;
    if (zeros[j] - zeros[i] > 0) {
      prod = 0;
    } else {
      uint64_t inv = fast_exp_mod(partial_prod[i], P - 2, P);
      prod = (partial_prod[j] * inv) % P;
    }
    printf("%lu ", prod);
  }
  printf("\n");
#endif /* if QUESITO == 3 */

  return 0;
}
