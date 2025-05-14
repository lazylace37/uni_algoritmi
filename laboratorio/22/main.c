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

  // struct node_t *parent;
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
  // node->parent = NULL;
  node->height = 1;
  return node;
}

void avl_free_node(node_t *node) {
  if (node == NULL)
    return;
  if (node->value != NULL)
    free(node->value);
  free(node);
}

void avl_free(node_t *node) {
  if (node == NULL) {
    return;
  }

  avl_free(node->left);
  avl_free(node->right);
  avl_free_node(node);
}

static inline int avl_node_height(node_t *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

void avl_recalculate_height(node_t *node) {
  if (node == NULL) {
    return;
  }

  int left_height = avl_node_height(node->left);
  int right_height = avl_node_height(node->right);
  node->height = 1 + (left_height > right_height ? left_height : right_height);
}

node_t *avl_rotate_right(node_t *x) {
  node_t *y = x->left;

  x->left = y->right;
  // if (y->right != NULL) {
  //   y->right->parent = x;
  // }
  // y->parent = x->parent;

  // if (x->parent == NULL) { // x è la radice
  //   // return y;
  // } else if (x == x->parent->left) {
  //   x->parent->left = y;
  // } else {
  //   x->parent->right = y;
  // }

  y->right = x;
  // x->parent = y;

  avl_recalculate_height(x);
  avl_recalculate_height(y);

  return y;
}

node_t *avl_rotate_left(node_t *x) {
  node_t *y = x->right;

  x->right = y->left;
  // if (y->left != NULL) {
  //   y->left->parent = x;
  // }
  // y->parent = x->parent;

  // if (x->parent == NULL) { // x è la radice
  //   // return y;
  // } else if (x == x->parent->left) {
  //   x->parent->left = y;
  // } else {
  //   x->parent->right = y;
  // }

  y->left = x;
  // x->parent = y;

  avl_recalculate_height(x);
  avl_recalculate_height(y);

  return y;
}

node_t *avl_rebalance(node_t *x) {
  if (x == NULL) {
    return x;
  }

  avl_recalculate_height(x);

  int left_height = avl_node_height(x->left);
  int right_height = avl_node_height(x->right);

  int diff = left_height - right_height;
  if (diff > 1) { // Altezza sottoalbero sx > altezza sottoalbero dx
    node_t *y = x->left;
    node_t *z1 = y->left;
    node_t *z2 = y->right;

    diff = avl_node_height(z1) - avl_node_height(z2);
    if (diff >= 0) {
      return avl_rotate_right(x);
    } else {
      x->left = avl_rotate_left(x->left); // rotazione a sinistra
      // if (x->left != NULL)
      //   x->left->parent = x;
      return avl_rotate_right(x);
    }
  } else if (diff < -1) { // Altezza sottoalbero sx < altezza sottoalbero dx
    node_t *y = x->right;
    node_t *z1 = y->left;
    node_t *z2 = y->right;

    diff = avl_node_height(z1) - avl_node_height(z2);
    if (diff <= 0) {
      return avl_rotate_left(x);
    } else {
      x->right = avl_rotate_right(x->right); // rotazione a destra
      // if (x->right != NULL)
      //   x->right->parent = x;
      return avl_rotate_left(x);
    }
  }

  return x; // Albero già bilanciato
}

node_t *avl_insert(node_t *x, int k, char *v) {
  // Utilizziamo una versione ricorsiva per l'inserimento
  // per aggiornare le altezze del nodi antenati
  // e ribilanciare se necessario

  if (x == NULL)
    return make_node(k, v);

  if (k < x->key) {
    x->left = avl_insert(x->left, k, v);
    // x->left->parent = x;
  } else if (k > x->key) {
    x->right = avl_insert(x->right, k, v);
    // x->right->parent = x;
  } else {
    assert(0 /* "Chiave già presente" */);
    return x;
  }

  // Aggiorna altezza nodo corrente dopo inserimento
  avl_recalculate_height(x);

  return avl_rebalance(x);
}

node_t *bst_minimum(node_t *node) {
  while (node->left != NULL) {
    node = node->left;
  }
  return node;
}

node_t *avl_remove(node_t *x, int k) {
  if (x == NULL) {
    return NULL;
  }

  if (k < x->key) {
    x->left = avl_remove(x->left, k);
    // if (x->left != NULL)
    //   x->left->parent = x;
  } else if (k > x->key) {
    x->right = avl_remove(x->right, k);
    // if (x->right != NULL)
    //   x->right->parent = x;
  } else {
    if (x->left == NULL) {
      node_t *right = x->right;
      avl_free_node(x);
      x = right;
    } else if (x->right == NULL) {
      node_t *left = x->left;
      avl_free_node(x);
      x = left;
    } else {
      node_t *successor = bst_minimum(x->right);
      x->key = successor->key;

      if (x->value != NULL)
        free(x->value);

      if (successor->value != NULL)
        x->value = strdup(successor->value);
      else
        x->value = NULL;

      x->right = avl_remove(x->right, successor->key);
      // if (x->right != NULL)
      //   x->right->parent = x;
    }
  }

  if (x == NULL) {
    return NULL;
  }

  avl_recalculate_height(x);

  return avl_rebalance(x);
}

node_t *avl_find(node_t *node, int k) {
  if (node == NULL) {
    return NULL;
  }

  if (node->key == k) {
    return node;
  } else if (k < node->key) {
    return avl_find(node->left, k);
  } else {
    return avl_find(node->right, k);
  }
}

void avl_print_polish_form(node_t *node) {
  if (node == NULL) {
    printf("NULL ");
    return;
  }

  printf("%d:%s:%d ", node->key, node->value, node->height);
  avl_print_polish_form(node->left);
  avl_print_polish_form(node->right);
}

int avl_height(node_t *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

void avl_clear(node_t **root) {
  if (*root == NULL) {
    return;
  }

  avl_free(*root);
  *root = NULL;
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
      root = avl_insert(root, key, value);
      printf("\n");
      break;
    }
    case REMOVE: {
      int key = parse_get_number(&it);
      root = avl_remove(root, key);
      printf("\n");
      break;
    }
    case FIND: {
      int key = parse_get_number(&it);
      node_t *node = avl_find(root, key);
      assert(node != NULL);
      printf("%s\n", node->value);
      break;
    }
    case CLEAR: {
      avl_clear(&root);
      printf("\n");
      break;
    }
    case SHOW: {
      avl_print_polish_form(root);
      printf("\n");
      break;
    }
    case HEIGHT: {
      printf("%d\n", avl_height(root));
      break;
    }
    case EXIT: {
      avl_free(root);
      printf("\n");
      return 0;
    }
    }
  }

  avl_free(root);
  return 0;
}
