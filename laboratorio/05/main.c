#include <stdio.h>
#include <string.h>

int main(void) {
  char input[256];
  scanf("%255[^\n]", input);

  size_t len = strlen(input);
  for (size_t i = 0; i < len / 2; i++) {
    char tmp = input[i];
    input[i] = input[len - i - 1];
    input[len - i - 1] = tmp;
  }
  printf("%s\n", input);
}
