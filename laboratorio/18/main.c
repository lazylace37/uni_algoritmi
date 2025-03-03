#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t node_t;
struct node_t {
  int value;
  struct node_t *left;
  struct node_t *right;
};

typedef enum { TK_NUMBER, TK_NULL } token_type;
typedef struct {
  token_type type;
  int value;
} token_t;

node_t *bst_build(token_t *tokens, size_t *i, size_t n) {
  if (tokens[*i].type == TK_NULL) {
    (*i)++;
    return NULL;
  }

  node_t *node = malloc(sizeof(node_t));
  node->value = tokens[*i].value;

  (*i)++;
  node->left = bst_build(tokens, i, n);
  node->right = bst_build(tokens, i, n);

  return node;
}

void bst_free(node_t *node) {
  if (node == NULL) {
    return;
  }

  bst_free(node->left);
  bst_free(node->right);
  free(node);
}

int check_if_bst(node_t *node, int min, int max) {
  if (node == NULL) {
    return 1;
  }

  if (node->value < min || node->value > max) {
    return 0;
  }

  return check_if_bst(node->left, min, node->value) &&
         check_if_bst(node->right, node->value, max);
}

int main(void) {
  // Leggi input
  char buf[1024];
  char *read = fgets(buf, sizeof(buf), stdin);
  if (read == NULL) {
    return 1;
  }

  ssize_t len = strlen(read);
  if (len > 0 && read[len - 1] == '\n') {
    read[len - 1] = '\0';
    len--;
  }

  if (len == 0) {
    assert(0); // Non dovrebbe succedere
    printf("1\n");
    return 1;
  } else if (len >= 4 && strcmp(read, "NULL") == 0) {
    // Un albero vuoto è un albero binario di ricerca
    printf("1\n");
    return 0;
  }

  // Converti input in token
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

      tokens[tokens_len++] = (token_t){.type = TK_NUMBER, .value = value};
    } else if (i + 3 < len && strncmp(read + i, "NULL", 4) == 0) {
      tokens[tokens_len++] = (token_t){.type = TK_NULL};
      i += 3 + 1; // Salta "NULL "
    }
  }

  // Costruisci un albero dai token in forma polacca
  size_t i = 0;
  node_t *root = bst_build(tokens, &i, tokens_len);

  int is_bst = check_if_bst(root, INT_MIN, INT_MAX);
  printf("%d\n", is_bst);

  bst_free(root);

  return 0;
}
