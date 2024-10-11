#include <stdio.h>

#define LENGTH(A) sizeof(A) / sizeof(A[0])
#define PRINT_ARRAY(A, n)                                                      \
  for (size_t i = 0; i < n; i++) {                                             \
    printf("%d ", A[i]);                                                       \
  }                                                                            \
  printf("\n");

void InsertionSort(int A[], size_t n) {
  for (size_t i = 1; i < n; i++) {
    printf("\n");
    printf("iter: %zu\n", i);
    int key = A[i];
    int j = i - 1;
    while (j >= 0 && A[j] > key) {
      printf("  Moving %d to %d\n", A[j], A[j + 1]);
      A[j + 1] = A[j];
      j--;
    }
    printf("  Copying the key %d to %d\n", key, A[j + 1]);
    A[j + 1] = key;
    PRINT_ARRAY(A, n);
  }
}

int main(void) {
  int A[] = {5, 2, 4, 6, 1, 3};
  PRINT_ARRAY(A, LENGTH(A));
  InsertionSort(A, LENGTH(A));
}
