#include <stdio.h>
#include <string.h>

#define LENGTH(A) sizeof(A) / sizeof(A[0])
#define PRINT_ARRAY(A, n)                                                      \
  for (size_t i = 0; i < n; i++) {                                             \
    printf("%d ", A[i]);                                                       \
  }                                                                            \
  printf("\n");

void Merge(int *A, int p, int r, int q) {
  int m = q - p + 1;
  int i = p, j = r + 1;
  int B[m];
  for (int k = 0; k < m; k++) {
    if (i <= r && j <= q) {
      if (A[i] < A[j]) {
        B[k] = A[i++];
      } else {
        B[k] = A[j++];
      }
    } else if (j > q) {
      B[k] = A[i++];
    } else {
      B[k] = A[j++];
    }
  }

  for (int k = 0; k < m; k++) {
    A[p + k] = B[k];
  }
  /*memcpy(A + p, B, m * sizeof(int));*/
}

void MergeSort(int *A, int p, int q) {
  if (p < q) {
    int r = (p + q) / 2;
    MergeSort(A, p, r);
    MergeSort(A, r + 1, q);
    Merge(A, p, r, q);
  }
}

int main(void) {
  int A[] = {5, 2, 4, 6, 1, 3};
  PRINT_ARRAY(A, LENGTH(A));
  MergeSort(A, 0, LENGTH(A) - 1);
  PRINT_ARRAY(A, LENGTH(A));
}
