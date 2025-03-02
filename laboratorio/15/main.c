#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t node_t;
struct node_t {
  int value;
  struct node_t *left;
  struct node_t *right;
};

typedef enum { NUMBER, NIL } token_type;
typedef struct {
  token_type type;
  int value;
} token_t;

node_t *build(token_t *tokens, size_t *i, size_t n) {
  if (tokens[*i].type == NIL) {
    (*i)++;
    return NULL;
  }

  node_t *node = malloc(sizeof(node_t));
  node->value = tokens[*i].value;

  (*i)++;
  node->left = build(tokens, i, n);
  node->right = build(tokens, i, n);

  return node;
}

void node_inorder_print(node_t *node) {
  if (node == NULL) {
    return;
  }

  node_inorder_print(node->left);
  printf("%d ", node->value);
  node_inorder_print(node->right);
}

int main(void) {
  char buf[1024];
  char *read = fgets(buf, sizeof(buf), stdin);
  if (read == NULL) {
    return 0;
  }

  ssize_t len = strlen(read);
  if (len > 0 && read[len - 1] == '\n') {
    read[len - 1] = '\0';
    len--;
  }

  if (strcmp(read, "NULL") == 0) {
    return 0;
  }

  // Parse into tokens
  token_t tokens[1024];
  int tokens_len = 0;
  for (int i = 0; i < len; i++) {
    if (isspace(read[i])) {
      continue;
    }

    if (isdigit(read[i])) {
      int sign = 1;
      if (i > 0 && read[i - 1] == '-') {
        sign = -1;
      }

      int value = 0;
      while (i < len && isdigit(read[i])) {
        value = value * 10 + read[i] - '0';
        i++;
      }
      value *= sign;

      tokens[tokens_len++] = (token_t){.type = NUMBER, .value = value};
    } else if (i + 3 < len && strncmp(read + i, "NULL", 4) == 0) {
      tokens[tokens_len++] = (token_t){.type = NIL};
      i += 3;
    }
  }

  // Build tree
  size_t i = 0;
  node_t *root = build(tokens, &i, tokens_len);
  node_inorder_print(root);
  printf("\n");

  return 0;
}
