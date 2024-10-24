#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char input[1 << 12];
  fgets(input, sizeof input, stdin);
  input[strlen(input) - 1] = '\0';

  int key;
  scanf("%d", &key);

  int vec[256];
  size_t n = 0;
  int bytes_read = 0, offset = 0;
  while (n < sizeof(vec) / sizeof(vec[0]) &&
         sscanf(input + offset, "%d%n", &vec[n], &bytes_read) > 0) {
    n++;
    offset += bytes_read;
  }

  if (n == 0) {
    printf("-1\n");
    return 0;
  }

  size_t low = 0, high = n - 1;
  int idx = -1;
  while (low <= high) {
    size_t mid = low + (high - low) / 2;
    if (vec[mid] == key) {
      idx = mid;
      break;
    } else if (vec[mid] > key) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  printf("%d\n", idx);
  return 0;
}
