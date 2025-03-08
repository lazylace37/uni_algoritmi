#include "command_parsing.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t node_t;
struct node_t {
  int key;
  char *value;

  struct node_t *parent;
  struct node_t *left;
  struct node_t *right;

  int height;
};

node_t *make_node(int k, char *v) {
  node_t *node = malloc(sizeof(node_t));
  node->key = k;
  node->value = v;
  node->left = NULL;
  node->right = NULL;
  node->parent = NULL;
  node->height = 1;
  return node;
}

void bst_recalculate_height(node_t *node) {
  if (node == NULL) {
    return;
  }

  int left_height = node->left == NULL ? 0 : node->left->height;
  int right_height = node->right == NULL ? 0 : node->right->height;
  node->height = 1 + (left_height > right_height ? left_height : right_height);
}

void bst_insert(node_t **root, int k, char *v) {
  node_t *y = NULL;
  node_t *x = *root;

  while (x != NULL) {
    y = x;
    if (k < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  node_t *z = make_node(k, v);
  z->parent = y;
  z->height = 1; // è una foglia

  if (y == NULL) {
    *root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }

  // Aggiorno l'altezza dei genitori
  node_t *tmp = y;
  while (tmp != NULL) {
    bst_recalculate_height(tmp);
    tmp = tmp->parent;
  }
}

node_t *bst_minimum(node_t *node) {
  while (node->left != NULL) {
    node = node->left;
  }
  return node;
}

node_t *bst_remove(node_t *node, int k) {
  if (node == NULL) {
    return NULL;
  }

  if (k < node->key) {
    node->left = bst_remove(node->left, k);
  } else if (k > node->key) {
    node->right = bst_remove(node->right, k);
  } else {
    if (node->left == NULL) {
      node_t *right = node->right;

      if (right != NULL) {
        right->parent = node->parent;
      }

      free(node);
      return right;
    } else if (node->right == NULL) {
      node_t *left = node->left;

      if (left != NULL) {
        left->parent = node->parent;
      }

      free(node);
      return left;
    } else {
      node_t *successor = bst_minimum(node->right);
      node->key = successor->key;
      node->value = successor->value;
      node->right = bst_remove(node->right, successor->key);
    }
  }

  // Aggiorno l'altezza dei genitori
  node_t *tmp = node;
  while (tmp != NULL) {
    bst_recalculate_height(tmp);
    tmp = tmp->parent;
  }

  return node;
}

node_t *bst_find(node_t *node, int k) {
  if (node == NULL) {
    return NULL;
  }

  if (node->key == k) {
    return node;
  } else if (k < node->key) {
    return bst_find(node->left, k);
  } else {
    return bst_find(node->right, k);
  }
}

void bst_clear(node_t **root) {
  if (*root == NULL) {
    return;
  }

  bst_clear(&(*root)->left);
  bst_clear(&(*root)->right);
  free(*root);
  *root = NULL;
}

void bst_print_polish_form(node_t *node) {
  if (node == NULL) {
    printf("NULL ");
    return;
  }

  printf("%d:%s:%d ", node->key, node->value, node->height);
  bst_print_polish_form(node->left);
  bst_print_polish_form(node->right);
}

int bst_height(node_t *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

void bst_free(node_t *node) {
  if (node == NULL) {
    return;
  }

  bst_free(node->left);
  bst_free(node->right);
  free(node);
}

int main(void) {
  node_t *root = NULL;

  char buf[1024];
  char *read;
  while ((read = fgets(buf, sizeof(buf), stdin)) != NULL) {
    ssize_t len = strlen(read);
    if (len > 0 && read[len - 1] == '\n') {
      read[len - 1] = '\0';
      len--;
    }

    input_iterator it = {read, 0, len};
    command_t command = parse_command(&it);

    switch (command) {
    case INSERT: {
      char *value = malloc(1024);
      int key = parse_get_number(&it);
      parse_get_string(&it, value, 1024);
      bst_insert(&root, key, value);
      printf("\n");
      break;
    }
    case REMOVE: {
      int key = parse_get_number(&it);
      root = bst_remove(root, key);
      printf("\n");
      break;
    }
    case FIND: {
      int key = parse_get_number(&it);
      node_t *node = bst_find(root, key);
      assert(node != NULL);
      printf("%s\n", node->value);
      break;
    }
    case CLEAR: {
      bst_clear(&root);
      printf("\n");
      break;
    }
    case SHOW: {
      bst_print_polish_form(root);
      printf("\n");
      break;
    }
    case HEIGHT: {
      printf("%d\n", bst_height(root));
      break;
    }
    case EXIT: {
      bst_free(root);
      printf("\n");
      return 0;
    }
    }
  }

  bst_free(root);
  return 0;
}
