#include <stdio.h>
#include <string.h>

int main(void) {
  char input[256];
  scanf("%255[^\n]", input);

  for (size_t i = 0; i < strlen(input); i++) {
    printf("%s\n", input + i);
  }
}
