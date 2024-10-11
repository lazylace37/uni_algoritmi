#include <stdio.h>

#define LENGTH(A) sizeof(A) / sizeof(A[0])
#define PRINT_ARRAY(A, n)                                                      \
  for (size_t i = 0; i < n; i++) {                                             \
    printf("%d ", A[i]);                                                       \
  }                                                                            \
  printf("\n");

void TestHalf(int A[], size_t n) {
  int el;
  int c = 0;
  for (size_t i = 0; i < n - 1; i++) {
    if (c == 0) {
      el = A[i];
      c++;
    } else if (A[i] == el) {
      c++;
    } else {
      c--;
    }
  }
  printf("Element: %d with count: %d\n", el, c);
  PRINT_ARRAY(A, n);
}

int main(void) {
  int A[] = {1, 3, 2, 2, 2, 4, 5, 2, 2, 1};
  PRINT_ARRAY(A, LENGTH(A));
  TestHalf(A, LENGTH(A));
}
