#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Versione 1: Θ(n^2)
 * Versione 2: Θ(n)
 */
#define VERSIONE 2

void parse_stdin(char *vec, size_t vec_size, size_t *n) {
  char input[1 << 12];
  fgets(input, sizeof input, stdin);
  input[strlen(input) - 1] = '\0';

  *n = 0;
  int bytes_read = 0, offset = 0;
  while (*n < vec_size &&
         sscanf(input + offset, "%c%n", &vec[*n], &bytes_read) > 0) {
    (*n)++;
    offset += bytes_read;
  }
}

size_t periodo_frazionario_quadr(const char *vec, size_t n) {
  for (size_t p = 1; p <= n; p++) {
    bool valid = true;
    for (size_t i = 0; i < n; i++) {
      if (vec[i] != vec[i % p]) {
        valid = false;
        break;
      }
    }
    if (valid) {
      return p;
    }
  }
  return n;
}

/*size_t periodo_frazionario(const char *vec, size_t n) {*/
/*  size_t r[n];*/
/*  r[0] = 0;*/
/*  for (size_t i = 1; i < n; i++) {      // Θ(n)*/
/*    size_t j = r[i - 1];                // max len del bordo precedente*/
/*    while (j > 0 && vec[i] != vec[j]) { // cerco il bordo corretto, se
 * esiste*/
/*      j = r[j - 1];*/
/*    }*/
/*    if (vec[i] == vec[j]) {*/
/*      r[i] = j + 1; // ho trovato un bordo più lungo, lo incremento*/
/*    } else {*/
/*      r[i] = 0; // non ho trovato un bordo, setto a 0*/
/*    }*/
/*  }*/
/*  return n - r[n - 1];*/
/*}*/

/**
 * Versione alternativa: più difficile da capire, ma forse la complessità è più
 * evidente.
 */
size_t periodo_frazionario(const char *vec, size_t n) {
  size_t r[n];
  r[0] = 0;

  size_t j = 0;
  size_t i = 1;
  while (i < n) {
    if (vec[i] == vec[j]) {
      r[i] = j + 1;
      i++;
      j++;
    } else {
      if (j == 0) { // non ho trovato un bordo, setto a 0
        r[i] = 0;
        i++;
      } else { // devo tornare indietro
        j = r[j - 1];
      }
    }
  }

  return n - r[n - 1];
}

#ifdef TEST
void test_periodo_frazionario(size_t (*calc_periodo_frazionario)(const char *,
                                                                 size_t)) {
  const char *s = "abcabcab";
  assert(calc_periodo_frazionario(s, strlen(s)) == 3);
  s = "aba";
  assert(calc_periodo_frazionario(s, strlen(s)) == 2);
  s = "abca";
  assert(calc_periodo_frazionario(s, strlen(s)) == 3);
  s = "ababaababa";
  assert(calc_periodo_frazionario(s, strlen(s)) == 5);
}

int main(void) {
  test_periodo_frazionario(periodo_frazionario_quadr);
  test_periodo_frazionario(periodo_frazionario);
  printf("All tests passed!\n");
  return 0;
}
#endif /* ifdef TEST */

#ifndef TEST
int main(void) {
  char vec[256];
  size_t n;
  parse_stdin(vec, sizeof vec / sizeof vec[0], &n);
  if (n <= 0) {
    printf("\n");
    return 1;
  }

#if VERSIONE == 1
  size_t p = periodo_frazionario_quadr(vec, n);
  printf("%zu\n", p);
#endif /* if VERSIONE == 1 */

#if VERSIONE == 2
  size_t p = periodo_frazionario(vec, n);
  printf("%zu\n", p);
#endif /* if VERSIONE == 2 */

  return 0;
}
#endif /* ifndef TEST */
