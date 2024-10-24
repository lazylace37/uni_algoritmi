#include <stdio.h>
#include <string.h>

int main(void) {
  char input[256];
  scanf("%255[^\n]", input);

  for (size_t i = strlen(input); i > 0; i--) {
    input[i] = '\0';
    printf("%s\n", input);
  }
}
